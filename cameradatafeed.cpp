#include "cameradatafeed.h"

CameraDataFeed::CameraDataFeed(QObject *parent) :
    QObject(parent)
{
    state = (State)0;
    fd = -1;
    QTextStream out(stdout);
    out << "device: " << device << endl;
    QTextStream(stdout) << "fd: " << fd << endl;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateData()));
    printState();
}
CameraDataFeed::~CameraDataFeed(){
    delete timer;
}
void CameraDataFeed::printState(){
    // I should use array lookup here I know.
    // I kept doing it wrong, and getting compile errors. Lost patience.
    QTextStream out(stdout);
    out << "state: " << (int)state << " states: ";
    for(int i = 0 ; i < 8 ; i++){
        State testState = (State)(1u << i);
        if(state & testState){
            switch(testState){
            case OPEN:
                out << "OPEN, ";
                break;
            case FMT:
                out << "FMT, ";
                break;
            case REQBUFS:
                out << "REQBUFS, ";
                break;
            case BUFFERS_ARRAY:
                out << "BUFFERS_ARRAY, ";
                break;
            case QUERYBUF:
                out << "QUERYBUF, ";
                break;
            case MMAP:
                out << "MMAP, ";
                break;
            case QBUF:
                out << "QBUF, ";
                break;
            case STREAM:
                out << "STREAM, ";
                break;
            case TIMER:
                out << "TIMER, ";
                break;
            default:
                out << "unknown state";
                break;
            }
        }
    }
    out << endl;
}


bool CameraDataFeed::openCamera(){
    QTextStream(stdout) << "Camera" << endl;
    if(state & OPEN)
    {
        QTextStream(stdout) << "camera already open" << endl;
        return false;
    }
    fd = open(device.toStdString().c_str(), O_NONBLOCK | O_RDWR); // Open non blocking becauce
    if (fd == -1)                                                 // getting frame from UI thread
    {
        perror("opening device");
        return false;
    }

    state = (State)(state | OPEN);
    return true;
}

bool CameraDataFeed::getControls(){
    QTextStream cout(stdout);
    cout << "getcontrols:" << endl;

//    __u8 * data = (__u8 *)calloc(64,sizeof(__u8));
    __u8 control = 1;
    __u16 size = 0;
    __u8 value = 0;
    __u8 * data = (__u8 *)&value;
    struct uvc_xu_control_query xquery;
    memset(&xquery, 0, sizeof(xquery));
    xquery.query = UVC_GET_LEN;
    xquery.size = 2;
    xquery.selector = control;
    xquery.unit = 5;
    xquery.data = (__u8 *)&size;

    if(-1 == ioctl(fd,UVCIOC_CTRL_QUERY,&xquery)){
        perror("UVC_GET_LEN");
    }
    cout << "UVC_GET_LEN:" << size << endl;

//    memset(&xquery, 0, sizeof(xquery));
    xquery.query = UVC_GET_MAX;
    xquery.size = size;
    xquery.selector = control;
    xquery.unit = 5;
    xquery.data = data;
    if(-1 == ioctl(fd,UVCIOC_CTRL_QUERY,&xquery)){
        perror("UVC_GET_MAX");
    }
    cout << "MAX:" << value << endl;

//    memset(&xquery, 0, sizeof(xquery));
    xquery.query = UVC_GET_MIN;
    xquery.size = size;
    xquery.selector = control;
    xquery.unit = 5;
    xquery.data = data;
    if(-1 == ioctl(fd,UVCIOC_CTRL_QUERY,&xquery)){
        perror("UVC_GET_MIN");
    }
    cout << "MIN:" << value << endl;

    memset(&xquery, 0, sizeof(xquery));
    xquery.query = UVC_GET_DEF;
    xquery.size = size;
    xquery.selector = control;
    xquery.unit = 5;
    xquery.data = data;
    if(-1 == ioctl(fd,UVCIOC_CTRL_QUERY,&xquery)){
        perror("UVC_GET_DEF");
    }
    cout << "Default:" << value << endl;

    memset(&xquery, 0, sizeof(xquery));
    xquery.query = UVC_GET_CUR;
    xquery.size = size;
    xquery.selector = control;
    xquery.unit = 5;
    xquery.data = data;
    if(-1 == ioctl(fd,UVCIOC_CTRL_QUERY,&xquery)){
        perror("UUVC_GET_CUR");
    }
    cout << "Current:" << value << endl;

    value = 16;
    memset(&xquery, 0, sizeof(xquery));
    xquery.query = UVC_SET_CUR;
    xquery.size = size;
    xquery.selector = control;
    xquery.unit = 5;
    xquery.data = data;
    if(-1 == ioctl(fd,UVCIOC_CTRL_QUERY,&xquery)){
        perror("UVC_SET_CUR");
    }
    cout << "Set Current data0:" << data[0] << " data1:" << data[1] << endl;

    memset(&xquery, 0, sizeof(xquery));
    xquery.query = UVC_GET_CUR;
    xquery.size = 1;
    xquery.selector = 1;
    xquery.unit = 5;
    xquery.data = data;
    if(-1 == ioctl(fd,UVCIOC_CTRL_QUERY,&xquery)){
        perror("UVC_GET_CUR 2");
    }
    cout << "Current data0:" << data[0] << " data1:" << data[1] << endl;

//UVC_GET_CUR
    struct v4l2_queryctrl qctrl;
    qctrl.id = V4L2_CTRL_FLAG_NEXT_CTRL;
    while (0 == ioctl (fd, VIDIOC_QUERYCTRL, &qctrl)) {
        out << "id: " << qctrl.id << " name: " << (char *)qctrl.name << endl;
        qctrl.id |= V4L2_CTRL_FLAG_NEXT_CTRL;
    }

    struct v4l2_query_ext_ctrl qext;
    qext.id = V4L2_CTRL_FLAG_NEXT_CTRL;
    while (0 == ioctl (fd, VIDIOC_QUERY_EXT_CTRL, &qext)) {
        out << "id: " << qext.id << " name: " << (char *)qext.name << endl;
        qctrl.id |= V4L2_CTRL_FLAG_NEXT_CTRL;
    }
    return true;
}

void CameraDataFeed::setControl(int control,int setting){
    __u8 selector = (__u8)control;
//    __u8 selector = 1;
    __u8 unit = 5;
    __u16 size = 1;
    __u8 value = (__u8)setting;
//    __u8 * data = (__u8 *)&value;
    struct uvc_xu_control_query xquery;
    memset(&xquery, 0, sizeof(xquery));
    xquery.query = UVC_SET_CUR;
    xquery.size = size;
    xquery.selector = selector;
    xquery.unit = unit;
    xquery.data = (__u8 *)&value;

    if(-1 == ioctl(fd,UVCIOC_CTRL_QUERY,&xquery)){
        perror("UVC_SET_CUR");
    }
}
void CameraDataFeed::setLaserPower(int value){
    setControl(1,value);
}
void CameraDataFeed::setIvcamSetting(int value){
    setControl(2,value);
}
void CameraDataFeed::setMrtoSetting(int value){
    setControl(3,value);
}
void CameraDataFeed::setFilterSetting(int value){
    setControl(5,value);
}
void CameraDataFeed::setConfidenceSetting(int value){
    setControl(6,value);
}

bool CameraDataFeed::setFormat(){
    QTextStream(stdout) << "FMT" << endl;
    if(! state & OPEN){
        QTextStream(stdout) << "must open camera first" << endl;
        return false;
    }else if(state & REQBUFS){
        QTextStream(stdout) << "cannot set format while buffers active" << endl;
        return false;
    }
    struct v4l2_format format;
    memset(&format, 0, sizeof(format));           // Clear struct
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;    // Required
    if (-1 == ioctl(fd, VIDIOC_G_FMT, &format)){  // Get video format
        perror("getting format");
        return false;
    }
    format.fmt.pix.width       = 640;             // Make sure it's the right size
    format.fmt.pix.height      = 480;
    if (-1 == ioctl(fd, VIDIOC_S_FMT, &format)){  // Set video format
        perror("setting format");
        return false;
    }
    state = (State)(state | FMT);
    return true;
}
bool CameraDataFeed::reqBuffers(){
    QTextStream(stdout) << "REQBUFS" << endl;
    uint dMask = OPEN | FMT;
    if(! ((state & dMask) == dMask)){
        QTextStream(stdout) << "depends OPEN & FMT" << endl;
        return false;
    } else if(state & REQBUFS){
        QTextStream(stdout) << "REQBUF already done" << endl;
        return false;
    }
    buffercount = 2; // Double buffer

    memset(&reqestBuffers, 0, sizeof(reqestBuffers));
    reqestBuffers.count = buffercount;
    reqestBuffers.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    reqestBuffers.memory = V4L2_MEMORY_MMAP;

    // this allocates memory, but does not return a pointer to it
    if (-1 == ioctl (fd, VIDIOC_REQBUFS, &reqestBuffers)) {
        perror("getting buffers");
        return false;
    }
    state = (State)(state | REQBUFS);
    return true;
}
bool CameraDataFeed::freeBuffers(){
    QTextStream(stdout) << "freebuffers" << endl;
    uint dMask = OPEN | REQBUFS;
    if((state & dMask) != dMask){
        QTextStream(stdout) << "depends OPEN & REQBUFS" << endl;
        return false;
    } else if(state & MMAP){
        QTextStream(stdout) << "cannot free buffers while mapped" << endl;
        return false;
    }
    memset(&reqestBuffers, 0, sizeof(reqestBuffers));
    reqestBuffers.count = 0; // Requesting zero buffers frees them
    reqestBuffers.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    reqestBuffers.memory = V4L2_MEMORY_MMAP;

    if (-1 == ioctl (fd, VIDIOC_REQBUFS, &reqestBuffers)) {
        perror("freeing buffers");
        return false;
    }
    state = (State)(state & ~REQBUFS);
    return true;
}

bool CameraDataFeed::newBufArray(){
    uint cMask = BUFFERS_ARRAY;
    if(state & cMask){
        QTextStream(stdout) << "buffers array already created" << endl;
        return false;
    }
    try{
        buffers = new v4l2_buffer[buffercount]();
    } catch(std::bad_alloc& err) {
        QTextStream(stderr) << "Cannot allocate buffers array: " << err.what() << endl;
        return false;
    }
    state = (State)(state | BUFFERS_ARRAY);
    return true;
}
bool CameraDataFeed::queryAllBuffers(){
    QTextStream(stdout) << "query all buffers" << endl;
    uint dMask = OPEN | REQBUFS | BUFFERS_ARRAY;
    if((state & dMask) != dMask){
        QTextStream(stdout) << "depends OPEN & REQBUFS & BUFFERS_ARRAY" << endl;
        return false;
    }else if(state & QUERYBUF){
        // can't double queryAll because it overwrites buffers array
        QTextStream(stdout) << "already queried" << endl;
        return false;
    }
    for (unsigned int i = 0; i < reqestBuffers.count; i++) {
        struct v4l2_buffer buffer;// = buffers[i];
        memset(&buffer, 0, sizeof(buffer));
        buffer.type = reqestBuffers.type;
        buffer.memory = reqestBuffers.memory;
        buffer.index = i;

        // Sets buffer.length and buffer.m.offset
        if (-1 == ioctl (fd, VIDIOC_QUERYBUF, &buffer)) {
            perror("did not get buffer parameters");
            return false;
        }
        // need info to mmap
        // this is the reason this action is needed
        buffers[i] = buffer;
    }
    state = (State)(state | QUERYBUF);
    return true;
}
bool CameraDataFeed::mMAP(){
    QTextStream(stdout) << "mmap" << endl;
    uint dMask = OPEN | REQBUFS | BUFFERS_ARRAY | QUERYBUF;
    uint cMask = MMAP | QBUF;
    if(! ((state & dMask) == dMask)){
        QTextStream(stdout) << "depends OPEN REQBUFS BUFFERS_ARRAY QUERYBUF" << endl;
        return false;
    } else if(state & cMask){
        QTextStream(stdout) << "MMAP already done" << endl;
        return false;
    }
    for (unsigned int i = 0; i < reqestBuffers.count; i++) {
        struct v4l2_buffer buffer = buffers[i];
        void * map = mmap(NULL,              // Kernel picks address
                     buffer.length,          // Driver dictates size
                     PROT_READ | PROT_WRITE, // Must match open flags
                     MAP_SHARED,             // Allows forking
                     fd,
                     buffer.m.offset);       // Driver dictated

        // use pointer field in buffer to store mmap pointer location
        // needed both for retrieving data, and freeing memory
        buffers[i].m.userptr = (long unsigned int)map;

        if (MAP_FAILED == map) {
            perror("mmap");
            for( ; i >= 1 ; i-- ){
                if (-1 == munmap((void *)buffers[i -1].m.userptr, buffers[i -1].length)){
                    perror("munmap");
                }
            }
//            delete[] buffers;
            return false;
        }
    }
    state = (State)(state | MMAP);
    return true;
}
bool CameraDataFeed::qbuf(){
    QTextStream(stdout) << "qbuf" << endl;
    uint dMask = OPEN | BUFFERS_ARRAY | MMAP;
    if(! ((state & dMask) == dMask)){
        QTextStream(stdout) << "depends OPEN BUFFERS_ARRAY MMAP" << endl;
        return false;
    } else if(state & STREAM){
        QTextStream(stdout) << "Don't do this while streaming is on" << endl;
        return false;
    }
    for (unsigned int i = 0; i < reqestBuffers.count; i++) {
        struct v4l2_buffer buffer = buffers[i];
        // Tell driver buffer is available for use
        if(-1 == ioctl(fd, VIDIOC_QBUF, &buffer)){
            perror("qbuf");
        }
    }
    state = (State)(state | QBUF);
    return true;
}
bool CameraDataFeed::startStream(){
    QTextStream(stdout) << "stream" << endl;
    uint dMask = OPEN | MMAP | QBUF;
    if(! ((state & dMask) == dMask)){
        QTextStream(stdout) << "depends OPEN MMAP QBUF" << endl;
        return false;
    } else if(state & STREAM){
        QTextStream(stdout) << "Already Streaming" << endl;
        return false;
    }
    if (-1 == ioctl(fd, VIDIOC_STREAMON, &reqestBuffers.type)){
        perror("start video stream");
        return false;
    }
    state = (State)(state | STREAM);
    return true;
}
bool CameraDataFeed::startClock(){
    if(state & TIMER){
        QTextStream(stdout) << "Already ticking" << endl;
        return false;
    }
    timer->start(1);
    state = (State)(state | TIMER);
    return true;
}
bool CameraDataFeed::stopClock(){
    uint dMask = TIMER;
    if(! ((state & dMask) == dMask)){
        QTextStream(stdout) << "Wasn't ticking already" << endl;
        return false;
    }
    timer->stop();
    state = (State)(state & ~TIMER);
    return true;
}
bool CameraDataFeed::stopStream(){
    QTextStream(stdout) << "stream off" << endl;
    uint dMask = OPEN | STREAM;
    if(! ((state & dMask) == dMask)){
        QTextStream(stdout) << "depends OPEN STREAM" << endl;
        return false;
    }
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(fd != -1){
        if (-1 == ioctl(fd, VIDIOC_STREAMOFF, &type)){
            perror("stop video stream");
            return false;
        }
    }
    state = (State)(state & ~STREAM);
    return true;
}
bool CameraDataFeed::freeMmap(){
    QTextStream(stdout) << "free mmap" << endl;
    uint dMask = OPEN | BUFFERS_ARRAY | MMAP;
    if(! ((state & dMask) == dMask)){
        QTextStream(stdout) << "depends OPEN BUFFERS_ARRAY MMAP" << endl;
        return false;
    } else if(state & STREAM){
        QTextStream(stdout) << "Cannot free mmap while streaming" << endl;
        return false;
    }
    bool errors = false;
    for (unsigned int i = 0; i < buffercount ; i++) {
        v4l2_buffer buffer = buffers[i];
        if (-1 == munmap((void *)buffer.m.userptr, buffer.length)){
            perror("munmap");
            errors = true;
        }
    }
    state = (State)(state & ~MMAP);
    return !errors;
}
bool CameraDataFeed::freeBufferArray(){
    uint dMask = OPEN | BUFFERS_ARRAY;
    uint cMask = MMAP;
    if(! ((state & dMask) == dMask)){
        QTextStream(stdout) << "depends OPEN BUFFERS_ARRAY" << endl;
        return false;
    } else if(state & cMask){
        QTextStream(stdout) << "cannot free buffer array while MMAP" << endl;
        return false;
    }
    delete[] buffers;
    state = (State)(state & ~(BUFFERS_ARRAY | QBUF | QUERYBUF));
    return true;
}
bool CameraDataFeed::closeCamera(){
    QTextStream(stdout) << "closing camera" << endl;
    uint dMask = OPEN;
    uint cMask = MMAP;
    if(! ((state & dMask) == dMask)){
        QTextStream(stdout) << "file not open" << endl;
        return false;
    } else if(state & cMask){
        QTextStream(stdout) << "cannot close program while MMAP" << endl;
        return false;
    }
    if (-1 == close(fd)){
        perror("close camera");
        return false;
    }
    fd = -1;
    state = (State)(state & ~(OPEN | FMT));
    return true;
}
void CameraDataFeed::startVideo(){
    openCamera();
    getControls();
    setFormat();
    reqBuffers();
    newBufArray();
    queryAllBuffers();
    mMAP();
    qbuf();
    startStream();
    startClock();
    printState();
}

void CameraDataFeed::stopVideo(){
    stopClock();
    stopStream();
    freeMmap();
    freeBufferArray();
    freeBuffers();
    closeCamera();
    printState();
}

void CameraDataFeed::updateData()
{
    uint dMask = OPEN | STREAM;
    if((state & dMask) != dMask){
        return;
    }
    struct v4l2_buffer dqbuf;
    memset(&dqbuf, 0, sizeof(dqbuf));
    dqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE; // cannot fetch buffer
    dqbuf.memory = V4L2_MEMORY_MMAP;          // without these fields set

    // get info of buffer that is oldest in queue "dequeue buffer"
    if (-1 == ioctl(fd, VIDIOC_DQBUF, &dqbuf)) {
       if(errno == EAGAIN){
           // frame not ready yet
       } else {
           perror("readbuf");
           QTextStream(stdout) << "err!" << endl;
       }
    } else {
        // VIDIOC_DQBUF doesn't actually return pointer to data when using mmap
        // Structure has index that can be used to look up pointer
        void * data = (void *)buffers[dqbuf.index].m.userptr;
        // Do something with data
        emit newData(data);

        // tell driver it can reuse framebuffer
        ioctl(fd, VIDIOC_QBUF, &dqbuf);
        // signal repaint();
    }
}

void CameraDataFeed::setCameraDevice(QString dev){
    device = dev;
}
