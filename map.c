#include <stdio.h>
#include "map.h"
#include <stdlib.h>
#include <memory.h>

typedef struct node_t{
    MapDataElement mapDataElement;
    MapKeyElement mapKeyElement;
    struct node_t* next;
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
    map->head = NULL;
    map->tail = NULL;

    return map;
}

void mapDestroy(Map map) {
    if(map->head != NULL) {
        while (map->head->next != NULL) {
            Node tmp = map->head->next;
            map->free_data(map->head->mapDataElement);
            map->free_key(map->head->mapKeyElement);
            free(map->head);
            map->head = tmp;
        }

        if(map->head->mapDataElement!=NULL)
            map->free_data(map->head->mapDataElement);          ///free last node
        if(map->head->mapKeyElement!=NULL)
            map->free_key(map->head->mapKeyElement);
        free(map->head);
    }


    free(map);
}

Map mapCopy(Map map){
    Map new_map = malloc(sizeof(*map));
    if (new_map == NULL){
        return NULL;
    }
    *new_map = *map;
    new_map->head = malloc(sizeof(*new_map->head ));
    if (new_map->head == NULL){
        mapDestroy(new_map);
        return NULL;
    }
    new_map->head->mapKeyElement = new_map->head->mapDataElement = NULL;
    for (map->tail = map->head->next, new_map->tail = new_map->head->next; map->tail->next != NULL
                                                                                      ; map->tail = map->tail->next){
        Node next_node = malloc(sizeof(*next_node));
        if (next_node == NULL){
            mapDestroy(new_map);
            return NULL;
        }
        new_map->tail = next_node;
        new_map->tail->mapKeyElement = map->key_copy(map->tail->mapKeyElement);
        new_map->tail->mapDataElement = map->data_copy(map->tail->mapDataElement);
        new_map->tail->next = NULL;
    }
    Node next_node = malloc(sizeof(*next_node)); //*  copy last node
    if (next_node == NULL){
        mapDestroy(new_map);
        return NULL;
    }
    new_map->tail = next_node;
    new_map->tail->mapKeyElement = map->key_copy(map->tail->mapKeyElement);
    new_map->tail->mapDataElement = map->data_copy(map->tail->mapDataElement);
    new_map->tail->next = NULL;
    return new_map;
}

int mapGetSize(Map map){
    return map->counter;
}

bool mapContains(Map map, MapKeyElement element){
    while (map->tail->next != NULL){
        if (map->tail->mapKeyElement == element){
            return true;
        }
        map->tail = map->tail->next;
    }
    if (map->tail->mapKeyElement == element){    //check last node
        return true;
    }

    return false;
}



MapResult createNewNode(Node *new_node,MapKeyElement keyElement, MapDataElement dataElement,Map map){
    *new_node = malloc(sizeof(struct node_t));
    if (*new_node == NULL) {
        return MAP_OUT_OF_MEMORY;
    }else{
        (*new_node)->mapDataElement = map->data_copy (dataElement);
        (*new_node)->mapKeyElement = map->key_copy(keyElement);
        (*new_node)->next = NULL;
    }
    return MAP_SUCCESS;
}
void addNewNodeAfterNode(Node new_node ,Node previousNode) {
    Node temp = previousNode->next;
    previousNode->next =new_node;
    new_node->next =  temp;
}
MapResult mapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement) {
    Node new_node=NULL;
    if(map->head == NULL){                    //first node in the list
        if (createNewNode(&new_node, keyElement, dataElement,map) == MAP_OUT_OF_MEMORY) {
            return MAP_OUT_OF_MEMORY;
        } else {
            map->counter++;
            map->head=new_node;
            map->tail=map->head;
            return MAP_SUCCESS;
        }
    }
    Node prevNode=map->head;
    for (map->tail = map->head; map->tail != NULL ; map->tail = map->tail->next) {
        if (map->compair_key(map->tail->mapKeyElement, keyElement) == 0) {//swap data
            map->tail->mapDataElement = map->data_copy(dataElement);
            map->tail->mapKeyElement = map->key_copy(keyElement);
            return MAP_SUCCESS;
        }
        if (map->compair_key(keyElement ,map->tail->mapKeyElement ) < 0) {//middle before this object
            if (createNewNode(&new_node, keyElement, dataElement,map) == MAP_OUT_OF_MEMORY) {
                return MAP_OUT_OF_MEMORY;
            } else {
                map->counter++;
                addNewNodeAfterNode(new_node,prevNode); //in the middle
                return MAP_SUCCESS;
            }
        }
        prevNode = map->tail;
      }
        map->tail=prevNode;
        if (map->compair_key( keyElement,map->tail->mapKeyElement) > 0){
            if (createNewNode(&new_node, keyElement, dataElement,map) == MAP_OUT_OF_MEMORY) { /// end of list
                return MAP_OUT_OF_MEMORY;
            } else {
                map->counter++;
                addNewNodeAfterNode(new_node, map->tail);
                return MAP_SUCCESS;
            }
        }
}

MapDataElement mapGet(Map map, MapKeyElement keyElement){
    for (map->tail = map->head; map->tail != NULL ; map->tail = map->tail->next) {
        if (map->compair_key(map->tail->mapKeyElement, keyElement) == 0){
            return map->tail->mapDataElement;
        }
    }
    return NULL;
}

MapResult mapRemove(Map map, MapKeyElement keyElement) {
    int flag = 0; // We check if the item is found//
    for (map->tail = map->head; map->tail->next != NULL ; map->tail = map->tail->next) {
        if (map->compair_key(map->tail->next->mapKeyElement, keyElement) == 0) {
            flag = 1;
            Node tmp = map->tail->next->next;
            map->free_data(map->tail->next->mapDataElement);
            map->free_key(map->tail->next->mapKeyElement);
            free(map->tail->next);                             //free key and data element
            map->tail->next = tmp;
            map->counter--;
        }
    }
    if (flag == 0){
        return MAP_ITEM_DOES_NOT_EXIST;
    }
}

MapKeyElement mapGetFirst(Map map) {
    if (map->head == NULL){
        return NULL;
    }
    map->tail = map->head;
    return  map->tail->mapKeyElement;
}

MapKeyElement mapGetNext(Map map){
    map->tail = map->tail->next;
    if(map->tail !=NULL)
        return map->tail->mapKeyElement;
    else
        return NULL;
}

MapResult mapClear(Map map){
    while(map->head != NULL)
    {
        Node tmp = map->head;
        map->head = map->head->next;
        map->free_data(tmp->mapDataElement);
        map->free_key(tmp->mapKeyElement);
    }
    map->free_data(map->head->mapDataElement);
    map->free_key(map->head->mapKeyElement);

    return MAP_SUCCESS;
}