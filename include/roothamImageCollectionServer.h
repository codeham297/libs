#ifndef ROOTHAM_ESP32_IMAGE_COLLECTION_SERVER_H
#define ROOTHAM_ESP32_IMAGE_COLLECTION_SERVER_H

#include "rootham_wifi.h"

void initImageCollectionServer(void *param);

#include <eloquent_esp32cam.h>
#include <eloquent_esp32cam/extra/esp32/wifi/sta.h>
#include <eloquent_esp32cam/viz/image_collection.h>

using eloq::camera;
using eloq::wifi;
using eloq::viz::collectionServer;

#endif // ROOTHAM_ESP32_IMAGE_COLLECTION_SERVER_H