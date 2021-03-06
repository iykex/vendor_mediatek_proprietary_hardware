#ifndef HWC_BLITER_H_
#define HWC_BLITER_H_

#include "DpBlitStream.h"

#include <utils/KeyedVector.h>
#include <utils/threads.h>

#include <ui/Rect.h>
#include "composer.h"

using namespace android;

struct HWLayer;
struct PrivateHandle;
class DisplayBufferQueue;

// ---------------------------------------------------------------------------

class BliterHandler : public LayerHandler
{
public:
    BliterHandler(int dpy, const sp<OverlayEngine>& ovl_engine);
    virtual ~BliterHandler();

    // set() in BliterHandler is used to create release fence
    // in order to notify upper producers that layers are already consumed
    virtual void set(struct hwc_display_contents_1* list, DispatcherJob* job);

    // process() in BliterHandler is used to utilize DpFramework
    // to rescale and rotate input layers and pass the results to display driver
    virtual void process(DispatcherJob* job);

    // nullop() in BliterHandler is used to
    // release all fence when frame queue is used
    virtual void nullop();

    virtual void boost();

    // dump() in BliterHandler is used to dump debug data
    virtual int dump(char* buff, int buff_len, int dump_level);

private:
    // setMirror() is used to create release fence for mirror source buffer
    // in order to notify previous engine know if it is consumed
    void setMirror(struct hwc_display_contents_1* list, DispatcherJob* job);

    // processPhyMirror() is used to utilize DpFramework
    // to rescale and rotate mirror source buffer
    // in order for post engine to do composition
    void processPhyMirror(DispatcherJob* job);

    // processPhyMirror() is used to utilize DpFramework
    // to rescale and rotate mirror source buffer
    // in order for codec to video encoding
    void processVirMirror(DispatcherJob* job);

    // releaseFence() is used to release source fence
    void releaseFence(int fd, unsigned int sync_marker, int ovl_in);

    // bypassBlit() is used to check if source buffer is dirty, or
    // there has any updated source buffer could be used
    bool bypassBlit(HWLayer* hw_layer, int ovl_in);

    struct BufferConfig
    {
        // identify if this config is valid
        bool is_valid;

        // check if need to do interlacing
        bool deinterlace;

        // buffer information in native handle
        unsigned int gralloc_width;
        unsigned int gralloc_height;
        unsigned int gralloc_stride;
        unsigned int gralloc_cbcr_align;
        unsigned int gralloc_vertical_stride;
        unsigned int gralloc_format;
        unsigned int gralloc_color_range;
        int gralloc_private_format;
        int gralloc_ufo_align_type;

        // src buffer setting for DpFramework
        DP_COLOR_ENUM src_dpformat;
        unsigned int  src_pitch;
        unsigned int  src_pitch_uv;
        unsigned int  src_height;
        unsigned int  src_plane;
        unsigned int  src_size[3];

        // dst buffer setting for DpFramework
        DP_COLOR_ENUM dst_dpformat;
        unsigned int  dst_width;
        unsigned int  dst_height;
        unsigned int  dst_pitch;
        unsigned int  dst_pitch_uv;
        unsigned int  dst_plane;
        unsigned int  dst_size[3];
        unsigned int  dst_ovl_id;
    };

    // getDisplayBufferQueue() is used to get workable display buffer queue
    sp<DisplayBufferQueue> getDisplayBufferQueue(PrivateHandle* priv_handle,
        BufferConfig* config,int ovl_in) const;

    // setDpConfig() is used to prepare configuration for DpFramwork
    status_t setDpConfig(PrivateHandle* priv_handle,
        BufferConfig* config, int ovl_in);

    struct InvalidateParam
    {
        int src_acq_fence_fd;

        buffer_handle_t src_handle;

        PrivateHandle* src_priv_handle;

        uint32_t transform;

        DpRect src_dp_roi;

        DpRect dst_dp_roi;

        Rect dst_crop;

        Rect src_base;

        Rect dst_base;

        int dst_rel_fence_fd;

        int dst_ion_fd;

        SECHAND dst_sec_handle;

        // used to profile latency
        bool is_mirror;

        unsigned int sequence;

        bool is_enhance;
    };

    // computeBufferCrop() is used to calculate src and dst crop region
    status_t computeBufferCrop(InvalidateParam* param,
        BufferConfig* config, int ovl_in, bool full_screen_update);

    // invalidate() is used to ask DpFramework to do bliting
    status_t invalidate(InvalidateParam* param, BufferConfig* config, int ovl_in);

    // processFillBlack() is used to clear destination buffer by scaling a small black buffer
    void processFillBlack(PrivateHandle* priv_handle, int* fence);

    // fillBkack() is used to clear destination buffer including secure buffer
    void fillBlack(buffer_handle_t handle, PrivateHandle* priv_handle, int* fence);

    // clearBackground() is used to check the orientation and clear buffer if needed
    void clearBackground(buffer_handle_t handle, int curr_orient, int* fence = NULL);

    mutable Mutex m_frame_queue_lock;

    typedef Vector<HWLayer*> HWLayerQueue;

    // FrameQueue is used to remember queued video frame
    struct FrameQueue
    {
        FrameQueue()
            : curr_hw_layer(NULL)
        { }

        HWLayerQueue queue;
        HWLayer* curr_hw_layer;
    };

    // m_frame_queues is a list for all FrameQueue
    DefaultKeyedVector<int, FrameQueue*> m_frame_queues;

    // m_dp_configs stores src/dst buffer and overlay input information
    BufferConfig* m_dp_configs;

    // m_blit_stream is a bit blit stream
    DpBlitStream m_blit_stream;
};

#endif // HWC_BLITER_H_
