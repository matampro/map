#include <stdio.h>
#include "map.h"
#include <stdlib.h>

typedef struct node_t{
    MapDataElement mapDataElement;
    MapKeyElement mapKeyElement;
    struct node_t* next;
    struct node_t* previous;
} *Node;

struct Map_t{
        int counter;
        Node head;
        Node tail;
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
    Map map = malloc(sizeof(*map));
    if (map == NULL){
        return NULL;
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
        return NULL;
    }
    map->tail = map->head;
    map->head->next = NULL;
    return map;
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
        return NULL;
    }
    *new_map = *map;
    new_map->head = malloc(sizeof(Node));
    if (new_map->head == NULL){
        mapDestroy(new_map);
        return NULL;
    }
    new_map->head->mapKeyElement = new_map->head->mapDataElement = NULL;
    for (map->tail = map->head->next, new_map->tail = new_map->head->next; map->tail->next; map->tail = map->tail->next){
        Node next_node = malloc(sizeof(Node));
        if (next_node == NULL){
            mapDestroy(new_map);
            return NULL;
        }
        new_map->tail = new_map->tail->next = next_node; // The next step//
        new_map->tail->mapKeyElement = map->key_copy(map->tail->mapKeyElement);
        new_map->tail->mapDataElement = map->data_copy(map->tail->mapDataElement);
        new_map->tail->next = NULL;
    }
    return new_map;
}

int mapGetSize(Map map){
    return map->counter;
}

bool mapContains(Map map, MapKeyElement element){
    while (map->tail->next != NULL){
        if (map->head->mapKeyElement == element){
            return true;
        }
        map->tail = map->tail->next;
    }
    return false;
}
MapResult createNewNode(Node new_node,MapKeyElement keyElement, MapDataElement dataElement){
    new_node = malloc(sizeof(struct node_t));
    if (new_node == NULL) {
        return MAP_OUT_OF_MEMORY;
    }else{
        new_node->mapDataElement = dataElement;
        new_node->mapKeyElement = keyElement;
    }
    return MAP_SUCCESS;
}
void addNewNodeAfterNode(Node new_node ,Node previousNode) {
    Node temp = previousNode->next;
    previousNode->next =new_node;
    new_node->next =  temp;
}
void addNewNodeBeforeNode(Node new_node ,Node previousNode) { // add new node at the beginning
    new_node->next =  previousNode->next;
    previousNode->next =new_node;
}
MapResult mapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement) {
    Node new_node;
    Node previousNode = NULL;
    for (map->tail = map->head; map->tail->next; map->tail = map->tail->next) {
        if (map->compair_key(map->tail->next->mapKeyElement, keyElement) == 0) {//swap data
            map->tail->next->mapDataElement = dataElement;
            map->tail->next->mapKeyElement = keyElement;
            return MAP_SUCCESS;
        }
        if (map->compair_key(map->tail->next->mapKeyElement, keyElement) < 0) {
            if (createNewNode(new_node, keyElement, dataElement) == MAP_OUT_OF_MEMORY) {
                return MAP_OUT_OF_MEMORY;
            } else {
                addNewNodeBeforeNode(new_node, map->tail->next);
                return MAP_SUCCESS;
            }
        }
        if (createNewNode(new_node, keyElement, dataElement) == MAP_OUT_OF_MEMORY) { /// end of list
                return MAP_OUT_OF_MEMORY;
        } else {
                addNewNodeAfterNode(new_node, map->tail->next);
                return MAP_SUCCESS;
        }
    }
}



/*
MapResult mapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement) {
    for (map->tail = map->head; map->tail->next; map->tail = map->tail->next) {
        if (map->compair_key(map->tail->next->mapKeyElement, keyElement) == 0) {
            map->tail->next->mapDataElement = dataElement;
            map->tail->next->mapKeyElement = keyElement;
            return MAP_SUCCESS;
        }
        if (map->compair_key(map->tail->next->mapKeyElement, keyElement) < 0) {
            Node new_node = malloc(sizeof(Node));
            if (new_node == NULL) {
                return MAP_OUT_OF_MEMORY;
            }
            map->counter++;
            new_node->next = map->tail->next;
            new_node->mapDataElement = dataElement;
            new_node->mapKeyElement = keyElement;
            map->tail->next = new_node;
            return MAP_SUCCESS;
        }
        if (map->compair_key(map->tail->mapKeyElement, keyElement) > 0 &&
            map->tail->next == NULL){
            Node new_node = malloc(sizeof(Node));
            if (new_node == NULL) {
                return MAP_OUT_OF_MEMORY;
            }
            map->counter++;
            new_node->next = NULL;
            new_node->mapDataElement = dataElement;
            new_node->mapKeyElement = keyElement;
            map->tail->next = new_node;
            return MAP_SUCCESS;

        }
    }
}

*/
MapDataElement mapGet(Map map, MapKeyElement keyElement){
    for (map->tail = map->head->next; map->tail->next; map->tail = map->tail->next) {
        if (map->compair_key(map->tail->mapKeyElement, keyElement) == 0){
            return map->tail->mapDataElement;
        }
    }
    return NULL;
}

MapResult mapRemove(Map map, MapKeyElement keyElement) {
    int flag = 0; // We check if the item is found//
    for (map->tail = map->head; map->tail->next; map->tail = map->tail->next) {
        if (map->compair_key(map->tail->next->mapKeyElement, keyElement) == 0) {
            flag = 1;
            Node tmp = map->tail->next->next;
            free(map->tail->next);
            map->tail->next = tmp;
            map->counter--;
        }
    }
    if (flag == 0){
        return MAP_ITEM_DOES_NOT_EXIST;
    }
}

MapKeyElement mapGetFirst(Map map) {
    if (map->head->next == NULL){
        return NULL;
    }
    map->tail = map->head->next;
    return map->tail->mapKeyElement;
}

MapKeyElement mapGetNext(Map map){
    map->tail = map->tail->next;
    return map->tail->mapKeyElement;
}

MapResult mapClear(Map map){
    for (map->tail = map->head; map->tail->next; map->tail = map->tail->next) {
        Node tmp = map->head->next;
        free(map->head);
        map->head = tmp;
    }
    return MAP_SUCCESS;
}