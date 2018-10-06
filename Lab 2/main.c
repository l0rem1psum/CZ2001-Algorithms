#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#define TABLE_SIZE 5
#define HT_PRIME_1 17
#define HT_PRIME_2 23
#define HT_INITIAL_BASE_SIZE 13


typedef struct {
    char* key;
    char* value;
} hash_item;

typedef struct {
    int size;
    int count;
    int base_size;
    hash_item** items;//why does it use double redirection?
} hash_table;

static hash_item* new_hash_item(const char *k, const char *v);
hash_table* new_hash_table();
static hash_table* new_sized_hash_table(const int base_size);
static void delete_hash_item(hash_item *i);
void delete_hash_table(hash_table *ht);
int is_prime(const int x);
int next_prime(int x);
static int ht_hash(const char *str, int a, int num_buckets);
static int ht_get_hash(const char *str, const int num_buckets, const int attempt);
static void resize_hashtable(hash_table *ht, const int base_size);
static void ht_resize_up(hash_table *ht);
static void ht_resize_down(hash_table *ht);
void ht_insert(hash_table *ht, const char *key, const char *value);
char *ht_search(hash_table *ht, const char *key);
void ht_delete(hash_table *ht, const char *key);
void print_hashtable(hash_table *ht);

static hash_item HT_DELETED_ITEM = {NULL, NULL};


static hash_item* new_hash_item(const char *k, const char *v){
    //const type modifier is used to prevent changing of pointers as the function only reads the content of the pointer and do not modify it.
    hash_item* i = malloc(sizeof(hash_item));
    i -> key = strdup(k);
    //Since string is stored in static memory by default. strdup() copies the char to the new location and assigns the address to i->key
    i -> value = strdup(v);
    return i;
}

hash_table* new_hash_table(){
    return new_sized_hash_table(HT_INITIAL_BASE_SIZE);
}

static hash_table* new_sized_hash_table(const int base_size){
    hash_table* ht = malloc(sizeof(hash_table));

    ht -> base_size = base_size;
    ht -> size = next_prime(ht -> base_size);
    ht -> count = 0;
    ht -> items = calloc((size_t)ht -> size, sizeof(hash_item*));

    return ht;
}

static void delete_hash_item(hash_item *i){
    free(i -> key);
    free(i -> value);
    free(i);
}

void delete_hash_table(hash_table *ht){
    for (int i = 0; i < ht -> size; i++){
        hash_item* item = ht -> items[i];
        if (item != NULL){
            delete_hash_item(item);
        }
    }
    free(ht -> items);
    free(ht);
}

int is_prime(const int x){
    if (x < 2){return -1;}
    if (x < 4){return 1;}
    if (x % 2 == 0){return 0;}
    for (int i = 3; i <= floor(sqrt((double)x)); i+=2){
        if((x % i) == 0){
            return 0;
        }
    }
    return 1;
}

int next_prime(int x){
    while(is_prime(x) != 1){
        x++;
    }
    return x;
}

static int ht_hash(const char *str, int a, int num_buckets){
    long hash = 0;
    const int len_str = strlen(str);
    for (int i = 0; i < len_str; i++){
        hash += (long)pow(a, len_str - (i + 1)) * str[i];
    }
    hash = hash % num_buckets;
    return (int)hash;
}

static int ht_get_hash(const char *str, const int num_buckets, const int attempt){
    const int hash_a = ht_hash(str, HT_PRIME_1, num_buckets);
    const int hash_b = ht_hash(str, HT_PRIME_2, num_buckets);
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

static void resize_hashtable(hash_table *ht, const int base_size){
    // A void function. No pointer to another hash table is returned. All the resizing is done on ht by creating a new resized hash table and updating (by swapping attributes) the old one before deleting the new one.
    if (base_size < HT_INITIAL_BASE_SIZE){return;}
    //To ensure the size of the resized hash table is larger or equal to the minimal allowable size for a hash table. This is defined in the macro.

    hash_table *new_ht = new_sized_hash_table(base_size);
    for (int i = 0; i < ht -> size; i++){
        hash_item *item = ht -> items[i];
        if(item != NULL && item != &HT_DELETED_ITEM){
            ht_insert(new_ht, item -> key, item -> value);
        }
    }

    ht -> base_size = new_ht -> base_size;
    // To update the new base size of the hash table.
    ht -> count = new_ht -> count;
    //This step is not necessary since the count of items in the new and old hash table are equal.

    const int tmp_size = ht -> size;
    ht -> size = new_ht -> size;
    new_ht -> size = tmp_size;
    //These steps are required as deleting a hash table requires the size of the hash table to be correct.

    hash_item **tmp_items = ht -> items;
    ht -> items = new_ht -> items;
    new_ht -> items = tmp_items;

    delete_hash_table(new_ht);
}

static void ht_resize_up(hash_table *ht){
    //This function and the resize down function wraps the resize_hashtable function to make it easier to use.
    const int new_size = ht -> size * 2;
    resize_hashtable(ht, new_size);
}

static void ht_resize_down(hash_table *ht){
    const int new_size = ht -> size / 2;
    resize_hashtable(ht, new_size);
}

void ht_insert(hash_table *ht, const char *key, const char *value){
    const int load_factor = ht -> count * 100 / ht -> size;
    if (load_factor > 70){
        ht_resize_up(ht);
    }
    //Before inserting another item into the hash table, the load factor is checked to ensure it's not greater than 0.7. This is to avoid collision. The load factor is multiplied by 100 to avoid floating point maths.


    hash_item *item = new_hash_item(key, value);
    //To construct the key-value pair.
    int index = ht_get_hash(item -> key, ht -> size, 0);

    //Use the hash function to get the index to store the key-value pair
    hash_item *cur = ht -> items[index];
    //Use a the item in the current index (which may be null or has been occupied).
    int i = 1;
    while(cur != NULL && cur != &HT_DELETED_ITEM){
        //Iterate through the whole hash table until the position is NULL
        if (!strcmp(cur -> key, key)){
            delete_hash_item(cur);
            ht -> items[index] = item;
            return;
        }
        index = ht_get_hash(key, ht -> size, i);
        //If occupied, get the index of the next position through the hash function. Attempt is incremented.
        cur = ht -> items[index];
        //Change index to i, if want to overwrite the existing entries in hash table.
        i++;
    }
    ht -> items[index] = item;
    //An empty position is found. The item construct at the beginning of the block is stored in the hash table.
    ht -> count++;
}

char *ht_search(hash_table *ht, const char *key){
    int index = ht_get_hash(key, ht -> size, 0);
    hash_item *item = ht -> items[index];
    int i = 1;
    while(item != NULL){
        if(item != &HT_DELETED_ITEM){
            if (!strcmp(item -> key, key)){
                return item -> value;
            }
        }
        index = ht_get_hash(key, ht -> size, i);
        item = ht -> items[index];
        i++;
    }
    return NULL;
}

void ht_delete(hash_table *ht, const char *key){
    const int load_factor = ht -> count * 100 / ht -> size;
    if (load_factor < 10){ht_resize_down(ht);}
    //Before deleting an item from the hash table, the load factor is checked to ensure it's not smaller than 0.1. The load factor is multiplied by 100 to avoid floating point maths.

    int index = ht_get_hash(key, ht -> size, 0);
    hash_item *item = ht -> items[index];
    int i = 1;
    while(item != NULL){
        if (item != &HT_DELETED_ITEM){
            if (!strcmp(item -> key, key)){
                delete_hash_item(item);
                ht -> items[index] = &HT_DELETED_ITEM;
            }
        }
        index = ht_get_hash(key, ht -> size, i);
        item = ht -> items[index];
        i++;
    }
    ht -> count--;
}

void print_hashtable(hash_table *ht){
    printf("---------------Printing hash table---------------\n");
    printf("The table (size: %d) has %d items with a load factor of %.2f.\n", ht -> size, ht -> count, (float)ht -> count / ht -> size);
    for (int i = 0; i < ht -> size; i++){
        if (ht -> items[i] == NULL){continue;}
        printf("Index: %d, Key: %s, Value: %s.\n", i, ht -> items[i] -> key, ht -> items[i] -> value);
    }
    printf("------------------Print Finished-----------------\n");
}

int main(){
    hash_table* ht = new_hash_table();
    ht_insert(ht, "ugGdM", "llhCN");
    ht_insert(ht, "kCdko", "rsCVZ");
    ht_insert(ht, "qzjXL", "kjniy");
    ht_insert(ht, "yaphO", "odQas");
    ht_insert(ht, "uuVxV", "YbAYK");
    ht_insert(ht, "gcIBT", "yDUYl");
    ht_insert(ht, "sCOrS", "LrSpk");
    ht_insert(ht, "CIzLV", "nPTCf");
    ht_insert(ht, "Rmusy", "OMlEk");
    ht_insert(ht, "fPptx", "MmxKX");
    ht_insert(ht, "YjPOG", "Focaw");
    ht_insert(ht, "rZxHv", "RbiQG");
    ht_insert(ht, "NACGj", "fZPZp");
    ht_insert(ht, "DqgLH", "HaHae");
    ht_insert(ht, "vxlxn", "CiqnX");
    ht_insert(ht, "drehL", "PySRO");
    ht_insert(ht, "fKzxa", "ZjiFI");
    ht_insert(ht, "KEWjq", "FNbgG");
    ht_insert(ht, "wZXCe", "FLYen");
    ht_insert(ht, "NUGNk", "MYSvr");
    ht_insert(ht, "uzmzA", "EZcUz");
    ht_insert(ht, "DNGWX", "NqnVl");
    ht_insert(ht, "DeZte", "wWRDh");
    ht_insert(ht, "Pojil", "yVWAu");
    ht_insert(ht, "ehNAK", "bZQWg");
    ht_insert(ht, "cUKPf", "QLyAM");
    ht_insert(ht, "DLLfR", "rFBEj");
    ht_insert(ht, "iihMJ", "ofImP");
    ht_insert(ht, "EpQJr", "aDOWe");
    ht_insert(ht, "ivZhz", "rrGAp");
    ht_insert(ht, "JLtaY", "VVfOr");
    ht_insert(ht, "JPdfx", "qvjDS");
    ht_insert(ht, "omFlF", "KNXbJ");
    ht_insert(ht, "ytezO", "gSUAi");
    ht_insert(ht, "wAiyk", "HzSud");
    ht_insert(ht, "CsNlT", "PzyhT");
    ht_insert(ht, "azNPA", "HQAAg");
    ht_insert(ht, "mCqFy", "jxQbP");
    ht_insert(ht, "hNNrZ", "zuLpT");
    ht_insert(ht, "HDDeJ", "aPPBs");
    ht_insert(ht, "TQsTH", "PJiIA");
    ht_insert(ht, "lPgZR", "EXKXz");
    ht_insert(ht, "zqPbq", "agHCP");
    ht_insert(ht, "ANCiv", "beBBp");
    ht_insert(ht, "parPt", "BKWLX");
    ht_insert(ht, "CpYLd", "VdsfF");
    ht_insert(ht, "XDeVJ", "gzyFq");
    ht_insert(ht, "XmEQA", "svgyN");
    ht_insert(ht, "HlTlc", "QoxEc");
    ht_insert(ht, "maEzO", "hDUqn");
    ht_insert(ht, "xiepn", "iMSoO");
    ht_insert(ht, "ZNaqp", "wHuAd");
    ht_insert(ht, "ofJHm", "PxcYa");
    ht_insert(ht, "XwZPI", "hEEte");
    ht_insert(ht, "MWzUN", "mSeRM");
    ht_insert(ht, "Hevgi", "GMcOr");
    ht_insert(ht, "NxYDm", "gXXxW");
    ht_insert(ht, "fqmyA", "pDgiM");
    ht_insert(ht, "vZODY", "FHMKP");
    ht_insert(ht, "KPLOy", "LLsEc");
    ht_insert(ht, "AGmvb", "NjYUc");
    ht_insert(ht, "OptCj", "yfeaF");
    ht_insert(ht, "HaNzx", "DNvhy");
    ht_insert(ht, "YgyAw", "BTjWU");
    ht_insert(ht, "sfQNJ", "bHVdz");
    ht_insert(ht, "YdQie", "xtoQi");
    ht_insert(ht, "NFcjA", "XZRFL");
    ht_insert(ht, "GdXAi", "JKbWK");
    ht_insert(ht, "uaGNv", "utUaJ");
    ht_insert(ht, "lkUTj", "jXPiR");
    ht_insert(ht, "QsbxC", "LXbbX");
    ht_insert(ht, "GFhAV", "Asyzc");
    ht_insert(ht, "lLVcT", "aYngc");
    ht_insert(ht, "pKlcR", "zXhOH");
    ht_insert(ht, "HxRWI", "FGdMt");
    ht_insert(ht, "rFNiU", "sGuPO");
    ht_insert(ht, "SEtAs", "zXAUW");
    ht_insert(ht, "tGzYe", "iMfcZ");
    ht_insert(ht, "NyHsW", "tMCNh");
    ht_insert(ht, "NZRCm", "pAhBC");
    //ht_insert(ht, "OruBo", "GVFHc");//Bad case
    ht_insert(ht, "QpJbx", "WpClf");
    ht_insert(ht, "LoBCV", "tVPaH");
    ht_insert(ht, "VqVep", "RlSUn");
    ht_insert(ht, "bPXkf", "mzPjn");
    ht_insert(ht, "wweIk", "ssIHf");
    ht_insert(ht, "HSvVj", "FvcYW");
    ht_insert(ht, "hVXQM", "WFlxs");
    ht_insert(ht, "pmUqi", "axmLR");
    ht_insert(ht, "fIYvZ", "uPFFL");
    ht_insert(ht, "GsOsT", "UabXz");
    ht_insert(ht, "VHpbd", "KUwIV");
    ht_insert(ht, "YsFWM", "fbLPs");
    ht_insert(ht, "dFtoR", "XeViT");
    ht_insert(ht, "jYxKB", "HRdTX");//
    ht_insert(ht, "cNnIa", "NUjLY");
    ht_insert(ht, "tzLQT", "GePsf");
    ht_insert(ht, "kglvQ", "xEDdE");
    ht_insert(ht, "gabQq", "JZfSV");
    ht_insert(ht, "KkhVs", "PmCdu");
    ht_insert(ht, "uWfjn", "LHMua");
    ht_insert(ht, "fBJsj", "lxPsz");
    ht_insert(ht, "jYEXD", "cQyrU");
    ht_insert(ht, "rmXUF", "NXpuD");
    ht_insert(ht, "aNUyh", "uGgDe");
    ht_insert(ht, "Yddfv", "mhvGQ");
    ht_insert(ht, "acwIv", "xVEVb");
    ht_insert(ht, "jsHPW", "TcHSZ");
    ht_insert(ht, "WRfxs", "yvCqD");
    ht_insert(ht, "JqESH", "OErnY");
    ht_insert(ht, "BXNDs", "ykfEa");
    ht_insert(ht, "Qsgnd", "TedAp");
    ht_insert(ht, "bvYRS", "sDvhb");
    ht_insert(ht, "CISgE", "qUgRE");
    ht_insert(ht, "dYYWA", "ACjlm");
    ht_insert(ht, "VgGHA", "INhGx");
    ht_insert(ht, "wFhMj", "ehgvf");
    ht_insert(ht, "JOTxi", "ufUip");
    ht_insert(ht, "NpBUP", "UaHmf");
    ht_insert(ht, "GuGXy", "hlUTr");
    ht_insert(ht, "wIuSa", "bFFWK");
    ht_insert(ht, "roxNn", "DTlKJ");
    ht_insert(ht, "wFHjS", "MtePT");
    ht_insert(ht, "mzekh", "OTOaE");
    ht_insert(ht, "cDzWP", "Lysiw");
    ht_insert(ht, "dAeOW", "zcNuA");
    ht_insert(ht, "TILpb", "cGNKM");
    ht_insert(ht, "ECpMr", "JdWyb");
    ht_insert(ht, "eNkFT", "BqhAD");
    ht_insert(ht, "NfhGy", "iVkPp");
    ht_insert(ht, "PGpTN", "egssA");
    ht_insert(ht, "vxpaX", "Ygyee");
    ht_insert(ht, "CqfrH", "JTMJu");
    ht_insert(ht, "NxrMH", "GKLrF");
    ht_insert(ht, "TZWaZ", "kuBxp");
    ht_insert(ht, "pPwXl", "HFdZi");
    ht_insert(ht, "gmBWT", "KSdNO");
    ht_insert(ht, "roOyN", "iVNXY");
    ht_insert(ht, "VzAXY", "TNnoD");
    ht_insert(ht, "enYPT", "qzrDo");
    ht_insert(ht, "oxlzy", "jpZBn");
    ht_insert(ht, "fahxi", "YtejD");
    ht_insert(ht, "xQXtb", "fRuMt");
    ht_insert(ht, "XXEhw", "ifHgE");
    ht_insert(ht, "oUzMy", "irkeC");
    ht_insert(ht, "aXtTX", "ZyrZf");
    ht_insert(ht, "srrMh", "WNKNl");
    ht_insert(ht, "zDLvV", "tcxfQ");
    ht_insert(ht, "yjvsr", "VqhEy");
    ht_insert(ht, "TyICG", "zfKEP");
    ht_insert(ht, "pvZkF", "naehg");
    ht_insert(ht, "uSaPa", "slHBs");
    ht_insert(ht, "wOKMB", "PgueL");
    ht_insert(ht, "BRaxk", "kyMyz");
    ht_insert(ht, "cSGqL", "AOLsk");
    ht_insert(ht, "SsJEm", "MYIgR");
    ht_insert(ht, "ULJrT", "GekEU");
    ht_insert(ht, "Iweqq", "dWyfB");
    ht_insert(ht, "vzxqe", "apgdl");
    ht_insert(ht, "NkCka", "DwUgK");
    ht_insert(ht, "ckZav", "kMyKb");
    ht_insert(ht, "uWGpz", "mxCNc");
    ht_insert(ht, "ywuZd", "ULWCF");
    ht_insert(ht, "kNcKI", "pmABb");
    ht_insert(ht, "PBGfQ", "YEUcL");
//    ht_insert(ht, "xFRYZ", "VFxTD");
    ht_insert(ht, "xghmU", "iisfa");
    ht_insert(ht, "GTyan", "iYzXO");
    ht_insert(ht, "RUhiw", "WxaBO");
    ht_insert(ht, "lCuuN", "VhMEQ");
    ht_insert(ht, "loHHB", "kXfXV");
    ht_insert(ht, "jzAMs", "ZachE");
    ht_insert(ht, "AaNiE", "sPlrT");
    ht_insert(ht, "GFooT", "dXLNK");
    ht_insert(ht, "wqDCA", "WvSlM");
    ht_insert(ht, "hUhsk", "AjdmX");
    ht_insert(ht, "cZhvm", "jFsJy");
    ht_insert(ht, "UdrIY", "BIinL");
    ht_insert(ht, "kNReC", "WIjoJ");
    ht_insert(ht, "pEXPA", "AuBHj");
    ht_insert(ht, "aoNGo", "EKeIr");
    ht_insert(ht, "ObJmd", "ijOIe");
    ht_insert(ht, "nXMNt", "YGbKZ");
    ht_insert(ht, "NGFJX", "qNBJf");
    ht_insert(ht, "mAeHb", "NKZpX");
    ht_insert(ht, "VTRjT", "ElmOk");
    ht_insert(ht, "VdSHS", "OyFpZ");
    ht_insert(ht, "lBeAj", "nJIdy");
    ht_insert(ht, "BqAgG", "MYgdp");
//    ht_insert(ht, "BFmNa", "KFkCX");
//    ht_insert(ht, "iEwxg", "tvnWf");
//    ht_insert(ht, "wMgfb", "EkGTp");
//    ht_insert(ht, "fEYNU", "lUwBc");
//    ht_insert(ht, "tjLcE", "OqZOw");
//    ht_insert(ht, "nnkBh", "ZhFFD");

    print_hashtable(ht);
    delete_hash_table(ht);
}
