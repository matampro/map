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
    map->counter = 0;
    map->head = malloc(sizeof(Node));
    if (map->head == NULL){
        free(map);
        return MAP_OUT_OF_MEMORY;
    }
    map->iterator = map->head;
    map->head->next = NULL;
    return MAP_SUCCESS;
}

void mapDestroy(Map map) {
    while (map->head->next != NULL){
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
    new_map=map;
    new_map->head = malloc(sizeof(Node));
    if (new_map->head == NULL){
        mapDestroy(map);
        return MAP_OUT_OF_MEMORY;
    }
    while (map->iterator->next != NULL){
        Node next_node = malloc(sizeof(Node));
        if (next_node == NULL){
            return MAP_OUT_OF_MEMORY;
        }
        new_map->iterator->mapKeyElement = map->key_copy(map->head->mapKeyElement);
        new_map->iterator->mapDataElement = map->data_copy(map->head->mapDataElement);
        new_map->iterator = new_map->iterator->next;
        map->iterator = map->iterator->next;
        new_map->iterator->next = next_node;
    }
    return MAP_SUCCESS;
}

int mapGetSize(Map map){
    return map->counter;
}

bool mapContains(Map map, MapKeyElement element){
    while (map->iterator->next != NULL){
        if (map->head->mapKeyElement == element){
            return true;
        }
        map->iterator = map->iterator->next;
    }
    return false;
}

MapResult mapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement){
    if (map->compair_key(map->iterator->mapKeyElement, keyElement) > NULL) {
        Node new_node = malloc(sizeof(Node));
        if (new_node == NULL) {
            return MAP_OUT_OF_MEMORY;
        }
        map->head = new_node;
        new_node->mapDataElement = dataElement;
        new_node->mapKeyElement = keyElement;
        new_node->next = map->iterator;
        map->iterator = map->iterator->next;
        return MAP_SUCCESS;
    }
    while (map->iterator->next != NULL){
        if (map->compair_key(map->iterator->mapKeyElement, keyElement) >= NULL){
            Node new_node = malloc(sizeof(Node));
            if (new_node == NULL){
                return MAP_OUT_OF_MEMORY;
            }
            map->head = new_node;
            new_node->mapDataElement = dataElement;
            new_node->mapKeyElement = keyElement;
            new_node->next = map->iterator;
            map->iterator->next->mapKeyElement = keyElement;
            map->iterator->next->mapDataElement = dataElement;
            return MAP_SUCCESS;
        }
    }
    return MAP_NULL_ARGUMENT;
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
        if (map->compair_key(map->head->mapKeyElement, keyElement) {
            // to remove the node and connect the befor and after nodes//
            map->iterator->mapKeyElement = NULL;
            map->iterator->mapDataElement = NULL;
        }
    }
    return MAP_ITEM_DOES_NOT_EXIST;
}


MapKeyElement mapGetFirst(Map map) {
    return map->head->mapKeyElement;
}

MapKeyElement mapGetNext(Map map){
    map->iterator = map->iterator->next;
    return map->iterator->mapKeyElement;
}

MapResult mapClear(Map map){
    while (map->head->next != NULL){
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

