#include "hashtable.c"
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>


// Example 1: Branch always taken
void test_branchalways() {
    size_t size = 1000000000;
    size_t x = 0;

    PAPI_hl_region_begin("branchalways");
    for (size_t i = 0; i < size; ++i) {
        if (pseudo_random()) {
            x++;
        }
    }
    PAPI_hl_region_end("branchalways");

    printf("branchalways done: %lu/%lu\n", x, size);
}

// Example 2: Branch taken 50% of the time
void test_branchhalf() {
    size_t size = 1000000000;
    size_t x = 0;

    PAPI_hl_region_begin("branchhalf");
    for (size_t i = 0; i < size; ++i) {
        if (pseudo_random() % 128 < 64) {
            x++;
        }
    }
    PAPI_hl_region_end("branchhalf");

    printf("branchhalf done: %lu/%lu\n", x, size);
}




struct cache_line {
    char data[64];
};


size_t test_read_seq(struct cache_line *memory, size_t size) {
    size_t x = 0;

    PAPI_hl_region_begin("read_seq");
    u_int64_t start = rdtsc();
    for (size_t i = 0; i < size; ++i) {
        x += memory[i].data[0];
    }
    PAPI_hl_region_end("read_seq");

    u_int64_t end = rdtsc();
    printf("read_seq done in %lld cycles\n", end - start);
    return x;
}

// Example 3: Random memory read
size_t test_read_rand(struct cache_line *memory, size_t size) {
    size_t x = 0;

    PAPI_hl_region_begin("read_rand");
    u_int64_t start = rdtsc();
    for (size_t i = 0; i < size; ++i) {
        size_t idx = pseudo_random() % size;
        x += memory[idx].data[0];
    }
    PAPI_hl_region_end("read_rand");

    u_int64_t end = rdtsc();
    printf("read_rand done in %lld cycles\n", end - start);
    return x;
}

//Example 4: hash table
size_t test_hash_table(hashmap *map, size_t size) {
    double load_factor = 0.75;
    size_t num_of_entries = (size_t)(load_factor * size);

    uint8_t (*keys)[KEY_SIZE] = malloc(size * KEY_SIZE);

    PAPI_hl_region_begin("hash");
    u_int64_t start = rdtsc();
    for (size_t i = 0; i < num_of_entries; ++i) {
        key_generate(keys[i]);
        assert(hashmap_insert(map, keys[i], i + 1));
    }
    PAPI_hl_region_end("hash");

    u_int64_t end = rdtsc();
    printf("compute hash done in %lld cycles\n", end - start);
    free(keys);

    return 0;
}

// Example 5: hash table look up
size_t test_hash_table_lookup(hashmap *map, size_t size) {
    uint64_t sum = 0;
    double load_factor = 0.75;
    size_t num_of_entries = (size_t)(load_factor * size);
    uint8_t (*keys)[KEY_SIZE] = malloc(size * KEY_SIZE);

    PAPI_hl_region_begin("hash_lookup");
    u_int64_t start = rdtsc();
    // Workload starts here
    for (size_t i = 0; i < num_of_entries; ++i) {
        size_t ki = pseudo_random() % size;
        sum += hashmap_lookup(map, keys[ki]);
    }
    PAPI_hl_region_end("hash_lookup");

    u_int64_t end = rdtsc();
    printf("hash look up done in %lld cycles\n", end - start);
    free(keys);

    return sum;
}



int main(int argc, char **argv) {
///////////////////////  Branch mispredictions  //////////////////////////////
    int init = PAPI_library_init(PAPI_VER_CURRENT);
    if (init != PAPI_VER_CURRENT) {
        fprintf(stderr, "Bad PAPI version");
        return 1;
    }

    test_branchalways();
    test_branchhalf();

///////////////////////  Cache misses    //////////////////////////////
    size_t size = 128 * 1024 * 1024;
    struct cache_line *region1 = (struct cache_line*) malloc(size*sizeof(struct cache_line));
    struct cache_line *region2 = (struct cache_line*) malloc(size*sizeof(struct cache_line));

    test_read_seq(region1, size);
    test_read_rand(region2, size);

    free(region1);
    free(region2);
    
    ///////////////////////   hash table   ///////////////////////

    hashmap *map = hashmap_init();
    test_hash_table(map, 1000);
    test_hash_table_lookup(map, 1000);
    free(map);

    return 0;
}