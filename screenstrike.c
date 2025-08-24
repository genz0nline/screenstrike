#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <xf86drm.h>
#include <xf86drmMode.h>

int main(void) {

    int card = open("/dev/dri/card1", 0);

    if (card <= 0) {
        fprintf(stderr, "ERROR: open: %s\n", strerror(errno));
        return 1;
    }

    drmSetMaster(card);
    drmModeResPtr res = drmModeGetResources(card);

    fprintf(stderr, "resources:\n"
            "\tFrame buffers: %d\n"
            "\t        CRTCs: %d\n"
            "\t   Connectors: %d\n"
            "\t     Encoders: %d\n",
            res->count_fbs,
            res->count_crtcs,
            res->count_connectors,
            res->count_encoders
            );


    drmModeConnectorPtr conn = NULL;

    for (size_t i = 0; i < res->count_connectors; i++) {
        conn = drmModeGetConnector(card, res->connectors[i]);
        if (conn->connection == DRM_MODE_CONNECTED) {
            break;
        }
    }

    if (conn == NULL) {
        fprintf(stderr, "No connectors are currently connected\n");
    }

    drmModeEncoder *enc;
    unsigned int i, j;

    for (i = 0; i < conn->count_encoders; ++i) {
        enc = drmModeGetEncoder(card, conn->encoders[i]);
        if (!enc) {
            continue;
        }

        /* iterate all global CRTCs */
        for (j = 0; j < res->count_crtcs; ++j) {
            /* check whether this CRTC works with the encoder */
            if (!(enc->possible_crtcs & (1 << j)))
                continue;

            /* Here you need to check that no other connector
                   * currently uses the CRTC with id "crtc". If you intend
                   * to drive one connector only, then you can skip this
                   * step. Otherwise, simply scan your list of configured
                   * connectors and CRTCs whether this CRTC is already
                   * used. If it is, then simply continue the search here. */
            if (res->crtcs[j] "is unused") {
                drmModeFreeEncoder(enc);
                return res->crtcs[j];
            }
        }

        drmModeFreeEncoder(enc);
    }

    /* cannot find a suitable CRTC */
    return -ENOENT;

    return 0;
}
