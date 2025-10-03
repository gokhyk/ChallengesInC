#include <stdio.h>
#include <errno.h>


#include <string.h>
#include <stdlib.h>

//#include <ctype.h>
//#include <stdarg.h>
//#include <stdint.h>


typedef struct {
    char *key;
    char *value;
} json_member;

typedef struct {
    size_t capacity;
    size_t count;
    json_member *members;
} json_object;


typedef struct {
    char *start;
    char *curr;
    char *end;
} parser;

static json_object *json_object_new(void) {
    json_object *jo = malloc(sizeof(*jo));
    jo->capacity = 0;
    jo->count = 0;
    jo->members = NULL;

    return jo;

}

static void object_add(json_object *jo, json_member jm) {
    if (jo->count == jo->capacity) {
        if (jo->capacity == 0) jo->capacity =8;
        else jo->capacity *= 2;
        jo->members = realloc(jo->members, jo->capacity * sizeof(*jo->members));
    }
    jo->members[jo->count].key = jm.key;
    jo->members[jo->count].value = jm.value;
    jo->count++;
}



//=========================================================================================================


static void skip_ws(parser *p) {
    while (p->curr && (*p->curr == ' ' || *p->curr == '\t' || *p->curr == '\n' || *p->curr == '\r')) {
        p->curr++;
    }
}


static json_object *parse_json_object(parser *p) {
    skip_ws(p);
    if (*p->curr != '{') return NULL;
    p->curr++;        
    json_object *jo = json_object_new();
    skip_ws(p);
    /*
    if (*p->curr == '}') {
        p->curr++;
        return jo;
    }*/
    while (1) {
        skip_ws(p);
        //if (*p->curr != '"') {printf("error there must be a key value\n"); free(jo); return NULL;}
        char *key = "key"; //TODO reading key
        skip_ws(p);
        //if (*p->curr != ':') {printf("error there must be a colon\n"); free(jo); return NULL;}
        skip_ws(p);
        char *value = "val"; //TODO reading value
        json_member jm = {0};
        jm.key = key;
        jm.value = value;
        object_add(jo, jm);
        if (*p->curr == '}') {p->curr++; break;}

    }
    return jo;

}


static json_object *parse_json(char *data, size_t len) {
    parser p;
    p.start = data;
    p.curr = data;
    p.end = data + len;

    skip_ws(&p);
    json_object *jo = parse_json_object(&p);
    return jo;
}


static char *read_all_file(FILE *f, size_t *out_len) {
    char *buffer = NULL;
    size_t len = 0;
    buffer = malloc(1024);
    size_t n = fread(buffer, 1, 1024, f);
    len += n;
    if (n == 0) {
        if (ferror(f)) { 
            perror("fread"); 
            free(buffer); 
            return NULL; }
    }
    
    *out_len = len;
    return buffer; 
}

int main(int argc, char *argv[]) {
    char *path = NULL;
    if (argc == 2) path = argv[1];
    FILE *f = stdin;
    if (path) {
        f = fopen(path, "rb");
        if (!f) { 
            fprintf(stderr, "cannot open '%s': %s\n", path, strerror(errno)); 
            return 1; 
        }
    }

    size_t len = 0;
    char *data = read_all_file(f, &len);

    if (path) fclose(f);
    if (!data) return 1;

    json_object *root = parse_json(data, len);
    if (root) printf("json is valid\n");
    else printf("json is not valid\n");
    free(data);
}