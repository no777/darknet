//
// Created by no777 on 17-5-22.
//

#include "pub-server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <netinet/in.h>  /* For htonl and ntohl */
#include <unistd.h>

#include <nanomsg/nn.h>
#include <nanomsg/pubsub.h>
#include "image.h"
#include "demo.h"
/*  The server runs forever. */

void limit_frames(float fps){
    static clock_t time = 0;
    float frameLimit = (1.0f/(float)fps);
    float frame = (float)(clock() - time) / CLOCKS_PER_SEC;
    if(frameLimit - frame > 0) {
        __useconds_t delay = (frameLimit - frame) * 1000*1000;
        usleep(delay);
    }
    time = clock();
}

int pub_server(const char *url)
{
    int fd;

    /*  Create the socket. */
    fd = nn_socket (AF_SP, NN_PUB);
    if (fd < 0) {
        fprintf (stderr, "nn_socket: %s\n", nn_strerror (nn_errno ()));
        return (-1);
    }

    /*  Bind to the URL.  This will bind to the address and listen
        synchronously; new clients will be accepted asynchronously
        without further action from the calling program. */

    if (nn_bind (fd, url) < 0) {
        fprintf (stderr, "nn_bind: %s\n", nn_strerror (nn_errno ()));
        nn_close (fd);
        return (-1);
    }

    /*  Now we can just publish results.  Note that there is no explicit
        accept required.  We just start writing the information. */

    for (;;) {
        uint8_t msg[2 * sizeof (uint32_t)];
        uint32_t secs, subs;
        int rc;

        secs = (uint32_t) time (NULL);
        subs = (uint32_t) nn_get_statistic (fd, NN_STAT_CURRENT_CONNECTIONS);

        secs = htonl (secs);
        subs = htonl (subs);

        memcpy (msg, &secs, sizeof (secs));
        memcpy (msg + sizeof (secs), &subs, sizeof (subs));

        image image = getImage();
        int len = image.w*image.h*image.c;
        limit_frames(30);
        rc = nn_send (fd, image.data, len, 0);
        if (rc < 0) {
            /*  There are several legitimate reasons this can fail.
                We note them for debugging purposes, but then ignore
                otherwise. */
            fprintf (stderr, "nn_send: %s (ignoring)\n",
                     nn_strerror (nn_errno ()));
        }
//        sleep(0.01);
    }

    /* NOTREACHED */
    nn_close (fd);
    return (-1);
}

