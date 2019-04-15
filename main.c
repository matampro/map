#include <stdio.h>
#include "map.h"
#include <stdlib.h>

typedef struct node_t{
    MapDataElement mapDataElement;
    MapKeyElement mapKeyElement;
    struct node_t* next;
} *Node;

struct Map_t{
        int counter;
        Node head;
        Node iterator;
        copyMapDataElements data_copy;
        copyMapKeyElements key_copy;
        compareMapKeyElements compair_key;
        freeMapDataElements free_data;
        freeMapKeyElements free_key;

};

Map mapCreate(copyMapDataElements copyDataElement,
              copyMapKeyElements copyKeyElement,
              freeMapDataElements freeDataElement,
              freeMapKeyElements freeKeyElement,
              compareMapKeyElements compareKeyElements) {
    Map map = malloc(sizeof(Map));
    if (map == NULL){
        return MAP_OUT_OF_MEMORY;
    }
    map->data_copy = copyDataElement;
    map->compair_key = compareKeyElements;
    map->free_data = freeDataElement;
    map->key_copy = copyKeyElement;
    map->free_key = freeKeyElement;
    map->counter = NULL;
    map->head = malloc(sizeof(Node));
    if (map->head == NULL){
        return MAP_OUT_OF_MEMORY;
    }
    map->iterator = map->head;
    map->head->next = NULL;
    return MAP_SUCCESS;
}

void mapDestroy(Map map) {
    MAP_FOREACH(void*, map->head, map){
        Node tmp = map->head->next;
        free(map->head);
        map->head = tmp;
    }
    free(map);
}

Map mapCopy(Map map){
    Map new_map = malloc(sizeof(map));
    if (new_map == NULL){
        return MAP_OUT_OF_MEMORY;
    }
    new_map = map;
    MAP_FOREACH(void*, map->head, map){
        new_map->head->mapKeyElement = map->head->mapKeyElement;
        new_map->head->mapDataElement = map->head->mapDataElement;
        new_map->head->next = map->head->next;
        new_map->head = new_map->head->next;
        map->head = map->head->next;
    }
}

int mapGetSize(Map map){
    printf("%d", map->counter);
}

bool mapContains(Map map, MapKeyElement element){
    MAP_FOREACH(void*, map->head, map){
        if (map->head->mapKeyElement == element){
            return true;
        }
    }
    return false;
}

MapResult mapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement){
    MAP_FOREACH(void*, map->head, map){
        if (compareMapKeyElements(map->head->mapKeyElement, keyElement) >= NULL){
            map->head->next->mapKeyElement = keyElement;
            map->head->next->mapDataElement = dataElement;
            return MAP_SUCCESS;
        }
    }
    return NULL;
}

MapDataElement mapGet(Map map, MapKeyElement keyElement){
    MAP_FOREACH(void*, &map->iterator, map){
        if (compareMapKeyElements(map->head->mapKeyElement, keyElement) == keyElement){
            return map->iterator->mapDataElement;
        }
    }
    return MAP_ITEM_DOES_NOT_EXIST;
}

MapResult mapRemove(Map map, MapKeyElement keyElement) {
    MAP_FOREACH(void*, &map->iterator, map){
        if (compareMapKeyElements(map->head->mapKeyElement, keyElement) == keyElement) {
            return map->iterator->mapKeyElement = NULL;
        }
    }
    return MAP_ITEM_DOES_NOT_EXIST;
}


MapKeyElement mapGetFirst(Map map) {
    map->iterator = map->head;
    return map->iterator->mapKeyElement;
}

MapKeyElement mapGetNext(Map map){
    map->iterator = map->iterator->next;
    return map->iterator->mapKeyElement;
}

MapResult mapClear(Map map){
    while(map->head->next != NULL) {
        Node tmp = map->head->next;
        free(map->head);
        map->head = tmp;
    }
}

//WE NEED TO WRIGHT TILL HERE//
MapDataElement copyMapDataElements(MapDataElement){

}

MapKeyElement copyMapKeyElements(MapKeyElement){

}

freeMapDataElements(MapDataElement){

}
freeMapKeyElements(MapKeyElement){

}

int compareMapKeyElements(MapKeyElement, MapKeyElement){

}

