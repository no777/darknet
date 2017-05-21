#ifndef GRPC_H
#define GRPC_H
#include "image.h"
#ifdef __cplusplus
extern "C" {
#endif
extern void RunServer();
extern void save_image_jpg(image p, const char *name);

#ifdef __cplusplus
}
#endif
#endif
