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
    map->counter = NULL;
    map->head = malloc(sizeof(Node));
    if (map->head == NULL) {
        return MAP_OUT_OF_MEMORY;
    }
    map->head->next = NULL;
    return MAP_SUCCESS;
}

void mapDestroy(Map map) {
    MAP_FOREACH(void*, itarator, map) {
        Node temp = itarator;
        free(itarator);
    }
}

Map mapCopy(Map map){

}

int mapGetSize(Map map){
    printf("%d", counter);
}

bool mapContains(Map map, MapKeyElement element){

}

MapResult mapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement){

}

MapDataElement mapGet(Map map, MapKeyElement keyElement){

}

MapResult mapRemove(Map map, MapKeyElement keyElement){

}


MapKeyElement mapGetFirst(Map map) {

}

MapKeyElement mapGetNext(Map map){

}

MapResult mapClear(Map map){

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

