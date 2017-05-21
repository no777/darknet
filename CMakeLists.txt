cmake_minimum_required(VERSION 3.5)
project(darknet)

set(CMAKE_CXX_STANDARD 11)

#set(CMAKE_MODULE_PATH cmake)
#set(CMAKE_MODULE_PATH "./cmake;${CMAKE_MODULE_PATH}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

add_definitions(-DOPENCV)

find_package(Protobuf REQUIRED)
find_package(GRPC REQUIRED)

#find_package ( ZMQ REQUIRED )
#IF(ZMQ_FOUND)
#    MESSAGE(STATUS "ZMQ_FOUND")
#    list(APPEND LIBRARIES ${ZMQ_LIBRARIES})
#endif()

find_package(nanomsg REQUIRED)
IF(nanomsg_FOUND)
    MESSAGE(STATUS "nanomsg found")
    list(APPEND LIBRARIES ${NANOMSG_LIBRARY})
endif()


FIND_PACKAGE(CUDNN REQUIRED)
IF(NOT CUDNN_FOUND)
    # CUDNN_INSTALL(5.1 "${Torch_INSTALL_LIB}" "${Torch_INSTALL_INCLUDE}" "${Torch_INSTALL_BIN}")
    # FIND_PACKAGE(CUDNN 5 EXACT REQUIRED)
    MESSAGE(STATUS "CuDNN 5.1 not found at install-time. Please make sure it's in LD_LIBRARY_PATH at runtime")
else()
#    message(CUDNN_LIBRARIES ${CUDNN_LIBRARIES})
    add_definitions(-DCUDNN)

ENDIF()

#thread
FIND_PACKAGE(Threads)

list(APPEND LIBRARIES ${CMAKE_THREAD_LIBS_INIT})

# CUDA
FIND_PACKAGE(CUDA)
if (CUDA_FOUND)
    message(STATUS "CUDA Version: " ${CUDA_VERSION_STRINGS})
    message(STATUS "CUDA Libararies: " ${CUDA_LIBRARIES} ${CUDA_CUDART_LIBRARIES})
    include_directories(SYSTEM ${CUDA_INCLUDE_DIRS})
#    list(APPEND LIBRARIES ${CUDA_LIBRARIES} ${CUDA_CUBLAS_LIBRARIES} ${CUDA_curand_LIBRARY} ${CUDA_cusparse_LIBRARY})
    list(APPEND LIBRARIES ${CUDA_LIBRARIES})
    list(APPEND LIBRARIES ${CUDA_CUDART_LIBRARIES})
    list(APPEND LIBRARIES ${CUDA_CUBLAS_LIBRARIES} ${CUDA_curand_LIBRARY} ${CUDA_cusparse_LIBRARY})
#    list(APPEND CUDA_NVCC_FLAGS "-std=c++11;-O2;-Xcompiler \"-fPIC\" ")
    list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_20,code=compute_20 ")
    list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_30,code=compute_30 ")
    list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_35,code=compute_35 ")
    list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_50,code=compute_50 ")
    list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_52,code=compute_52 ")
#    set(CUDA_PROPAGATE_HOST_FLAGS OFF)
    add_definitions(-DGPU)
    cuda_include_directories(src)
else()
    list(APPEND LIBRARIES "m")
endif()

FIND_PACKAGE(OpenCV REQUIRED)
if (OpenCV_FOUND)
    message(STATUS "OpenCV Found" ${OpenCV_LIBS})
    #message(STATUS "OpenCV Found" ${OpenCV_INCLUDE_DIR} ${OpenCV_LIBS})
endif()

set(SRC_DIR src)

set(SRC_FILES
        src/activation_layer.c
        src/activation_layer.h
        src/activations.c
        src/activations.h
        src/art.c
        src/avgpool_layer.c
        src/avgpool_layer.h
        src/batchnorm_layer.c
        src/batchnorm_layer.h
        src/blas.c
        src/blas.h
        src/box.c
        src/box.h
        src/captcha.c
        src/cifar.c
        src/classifier.c
        src/classifier.h
        src/coco.c
        src/col2im.c
        src/col2im.h
        src/compare.c
        src/connected_layer.c
        src/connected_layer.h
        src/convolutional_layer.c
        src/convolutional_layer.h
        src/cost_layer.c
        src/cost_layer.h
        src/crnn_layer.c
        src/crnn_layer.h
        src/crop_layer.c
        src/crop_layer.h
        src/cuda.c
        src/cuda.h
        src/darknet.c
        src/data.c
        src/data.h
        src/deconvolutional_layer.c
        src/deconvolutional_layer.h
        src/demo.c
        src/demo.h
        src/detection_layer.c
        src/detection_layer.h
        src/detector.c
        src/dice.c
        src/dropout_layer.c
        src/dropout_layer.h
        src/gemm.c
        src/gemm.h
        src/go.c
#        src/greeter_client.cc
#        src/grpc.cc
        src/gru_layer.c
        src/gru_layer.h
        src/im2col.c
        src/im2col.h
        src/image.c
        src/image.h
        src/layer.c
        src/layer.h
        src/list.c
        src/list.h
        src/local_layer.c
        src/local_layer.h
        src/lsd.c
        src/matrix.c
        src/matrix.h
        src/maxpool_layer.c
        src/maxpool_layer.h
        src/network.c
        src/network.h
        src/nightmare.c
        src/normalization_layer.c
        src/normalization_layer.h
        src/option_list.c
        src/option_list.h
        src/parser.c
        src/parser.h
        src/region_layer.c
        src/region_layer.h
        src/regressor.c
        src/reorg_layer.c
        src/reorg_layer.h
        src/rnn.c
        src/rnn_layer.c
        src/rnn_layer.h
        src/rnn_vid.c
        src/route_layer.c
        src/route_layer.h
        src/rpc.h
        src/shortcut_layer.c
        src/shortcut_layer.h
        src/softmax_layer.c
        src/softmax_layer.h
        src/stb_image.h
        src/stb_image_write.h
        src/super.c
        src/swag.c
        src/tag.c
        src/tree.c
        src/tree.h
        src/utils.c
        src/utils.h
        src/voxel.c
        src/writing.c
        src/yolo.c
        helloworld.grpc.pb.cc
        helloworld.grpc.pb.h
        helloworld.pb.cc
        helloworld.pb.h an_empty_file.cpp src/pub-server.c src/pub-server.h)
FILE(GLOB CU_SRC_FILES ${SRC_DIR}/*.cu)
ADD_LIBRARY(opencv_dep_cudart STATIC an_empty_file.cpp)

#add_executable(darknet ${SOURCE_FILES})

#message(STATUS "LIBRARIES " ${LIBRARIES})
if (CUDA_FOUND)
    message(STATUS "CUDA FOUND")
    cuda_add_library(dknet ${SRC_FILES} ${CU_SRC_FILES})
    target_link_libraries(dknet ${LIBRARIES} ${OpenCV_LIBS} ${PROTOBUF_LIBRARIES} ${GRPC_LIBRARIES} ${CUDNN_LIBRARIES} )
    cuda_add_executable(darknet ${SRC_DIR}/darknet.c)
    target_link_libraries(darknet ${LIBRARIES} ${OpenCV_LIBS} dknet)
#    cuda_add_library(pydarknet SHARED ${SRC_DIR}/pydarknet.cpp)

#    target_link_libraries(pydarknet dknet opencv_dep_cudart ${LIBRARIES} ${Boost_LIBRARIES} ${PYTHON_LIBRARIES} ${OpenCV_LIBS})
else ()
    add_library(dknet ${SRC_FILES})
    target_link_libraries(dknet ${LIBRARIES} ${OpenCV_LIBS} ${PROTOBUF_LIBRARIES} ${GRPC_LIBRARIES})
    add_executable(darknet ${SRC_DIR}/darknet.c)
    target_link_libraries(darknet ${LIBRARIES} ${OpenCV_LIBS} dknet)
#    add_library(pydarknet SHARED  ${SRC_DIR}/pydarknet.cpp)
#    target_link_libraries(pydarknet ${LIBRARIES} ${Boost_LIBRARIES} ${PYTHON_LIBRARIES} ${OpenCV_LIBS} dknet)
endif()
add_executable(sub-client ${SRC_DIR}/sub-client.c)
target_link_libraries(sub-client ${NANOMSG_LIBRARY})