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
        compareMapKeyElements compare_key;
        freeMapDataElements free_data;
        freeMapKeyElements free_key;
};

Map mapCreate(copyMapDataElements copyDataElement,
              copyMapKeyElements copyKeyElement,
              freeMapDataElements freeDataElement,
              freeMapKeyElements freeKeyElement,
              compareMapKeyElements compareKeyElements) {
    Map map = malloc( sizeof(*Map) );/// Map is map_t *, will give small size
    if (map == NULL){
        return MAP_OUT_OF_MEMORY;
    }
    map->data_copy = copyDataElement;
    map->compare_key = compareKeyElements;
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
    Map new_map = malloc(sizeof(*map));
    if (new_map == NULL){    
        return MAP_OUT_OF_MEMORY;
    }

    *new_map=*map; /// pointer copy is not value copy
    new_map->head = malloc(sizeof(Node));
    if (new_map->head == NULL){
        mapDestroy(new_map);
        return MAP_OUT_OF_MEMORY;
    }
    while (map->iterator->next != NULL){
        Node next_node = malloc(sizeof(Node));
        if (next_node == NULL){
            mapDestroy(new_map);
            return MAP_OUT_OF_MEMORY;
        }
        new_map->iterator->mapKeyElement = map->key_copy(map->iterator->mapKeyElement);
        new_map->iterator->mapDataElement = map->data_copy(map->iterator->mapDataElement);
        new_map->iterator = new_map->iterator->next;
        map->iterator = map->iterator->next;
        new_map->iterator->next = NULL;
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
/*void* createNewNode(Map map, MapKeyElement keyElement, MapDataElement dataElement, Node current_node){
    Node new_node = malloc(sizeof(*Node));
    if (new_node == NULL) {
        return MAP_OUT_OF_MEMORY;
    }
    map->head = new_node;
    new_node->mapDataElement = dataElement;
    new_node->mapKeyElement = keyElement;

    new_node->next = map->iterator;
    map->iterator = map->iterator->next; //why ??
    return MAP_SUCCESS;
}
map->iterator = map->iterator->next;
map->counter++;
while (map->iterator->next != NULL){
if (map->compair_key(map->iterator->mapKeyElement, keyElement) == 0){
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
*/
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
void addNewNodeAfterNode(Node new_node ,Node prevIterator) {
    new_node->next =  prevIterator->next;
    prevIterator->next =new_node;
}
/*void addNewNodeBefore(Node new_node ,Node prevIterator) { // add new node at the beginning
    new_node->next =  prevIterator->next;
    prevIterator->next =new_node;
}*/
MapResult mapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement) {
    Node new_node;
    Node prevIterator = NULL;
    for (map->iterator = map->head; map->iterator->next != NULL; map->iterator = map->iterator->next) {

        if (map->compare_key(map->iterator->mapKeyElement, keyElement) == 0) {//swap data
            map->iterator->mapDataElement = dataElement;
            map->iterator->mapKeyElement = keyElement;
            return MAP_SUCCESS;
        }
        if (map->compare_key(map->iterator->mapKeyElement, keyElement) > 0 &&
                map->iterator->next == NULL) {/// end of list
            if (createNewNode(new_node, keyElement, dataElement)== MAP_OUT_OF_MEMORY){
                return MAP_OUT_OF_MEMORY;
            }else{
                addNewNodeAfterNode(new_node ,map->iterator);
                return MAP_SUCCESS;
            }
        }
        if (map->compare_key(map->iterator->mapKeyElement, keyElement) < 0  ) {
            if( prevIterator != NULL){///middle of queue
                if (createNewNode(new_node, keyElement, dataElement)== MAP_OUT_OF_MEMORY){
                    return MAP_OUT_OF_MEMORY;
                }else{
                    addNewNodeAfterNode(new_node ,map->iterator);
                    return MAP_SUCCESS;
                }
            }else{///start of queue
                if (createNewNode(new_node, keyElement, dataElement)== MAP_OUT_OF_MEMORY){
                    return MAP_OUT_OF_MEMORY;
                }else{
                    addNewNodeAfterNode(new_node ,map->head);
                    return MAP_SUCCESS;
                }
            }
        }
        prevIterator = map->iterator;
    }
    return MAP_NULL_ARGUMENT; //should not get here
}

MapDataElement mapGet(Map map, MapKeyElement keyElement){
    while (map->iterator != NULL){
        if (compareMapKeyElements(map->iterator->mapKeyElement, keyElement) == 0){
            return map->iterator->mapDataElement;
        }
    }
    return NULL; // MAP_ITEM_DOES_NOT_EXIST
}

MapResult mapRemove(Map map, MapKeyElement keyElement) {
    while (map->iterator->next != NULL){
        if (map->compare_key(map->iterator->next->mapKeyElement, keyElement) {
            Node temp = map->iterator->next->next;
            free(map->iterator->next)
            map->iterator->next = temp;
        }
        map->iterator = map->iterator->next;
    }
    return MAP_ITEM_DOES_NOT_EXIST;
}


MapKeyElement mapGetFirst(Map map) {
    map->iterator = map->head;
    return map->itarator->mapKeyElement;
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
