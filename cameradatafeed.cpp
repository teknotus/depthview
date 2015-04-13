#include "cameradatafeed.h"

CameraDataFeed::CameraDataFeed(QObject *parent) :
    QObject(parent),out(stdout)
{
    state = (State)0;
    fd = -1;
//    fifo_filename = QString("/tmp/depthview/fifo");
    takeSnap = false;
    memset(&v4l2Format,0,sizeof(v4l2_format));
    depthMin = 0;
    depthMax = 0xffff;
    depthMask = 0xffff;
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
    out << "Camera" << endl;
    if(state & OPEN)
    {
        out << "camera already open" << endl;
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
    out << "getcontrols:" << endl;

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

//    struct v4l2_query_ext_ctrl qext;
//    qext.id = V4L2_CTRL_FLAG_NEXT_CTRL;
//    while (0 == ioctl (fd, VIDIOC_QUERY_EXT_CTRL, &qext)) {
//        out << "id: " << qext.id << " name: " << (char *)qext.name << endl;
//        qctrl.id |= V4L2_CTRL_FLAG_NEXT_CTRL;
//    }
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
    out << "FMT" << endl;
    if(! state & OPEN){
        out << "must open camera first" << endl;
        return false;
    }else if(state & REQBUFS){
        out << "cannot set format while buffers active" << endl;
        return false;
    }
    struct v4l2_fmtdesc fmtdesc;
    int i,e,r, trys;
    i = e = r = 0;
    __u8 * pixfmt = new __u8[5];
    pixfmt[4] = 0;
    trys = 255;
    while(trys-- > 0){
        memset(&fmtdesc, 0, sizeof(fmtdesc));
        fmtdesc.index = i;
        fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        if (-1 == ioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc)){  // Get video format
            perror("enum fmt");
            break;
        }
        *((__u32 *)pixfmt) = fmtdesc.pixelformat;
        printf("index: %d format: %s description: %s \n",
               i, pixfmt /*fmtdesc.pixelformat */, fmtdesc.description);
        i++;
    }
    delete[] pixfmt;
    struct v4l2_format pixFormat;
    memset(&pixFormat, 0, sizeof(pixFormat));           // Clear struct
    pixFormat.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;    // Required
    if (-1 == ioctl(fd, VIDIOC_G_FMT, &pixFormat)){  // Get video format
        perror("getting format");
        return false;
    }
    pixFormat.fmt.pix.pixelformat = fourcc;

    if(fourcc == YUYV){
        pixFormat.fmt.pix.width       = 1920;
        pixFormat.fmt.pix.height      = 1080;
    } else {
        pixFormat.fmt.pix.width       = 640;
        pixFormat.fmt.pix.height      = 480;
    }
    if (-1 == ioctl(fd, VIDIOC_S_FMT, &pixFormat)){  // Set video format
        perror("setting format");
        return false;
    }
    v4l2Format = pixFormat;
    state = (State)(state | FMT);
    return true;
}
bool CameraDataFeed::reqBuffers(){
    out << "REQBUFS" << endl;
    uint dMask = OPEN | FMT;
    if(! ((state & dMask) == dMask)){
        out << "depends OPEN & FMT" << endl;
        return false;
    } else if(state & REQBUFS){
        out << "REQBUF already done" << endl;
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
    out << "freebuffers" << endl;
    uint dMask = OPEN | REQBUFS;
    if((state & dMask) != dMask){
        out << "depends OPEN & REQBUFS" << endl;
        return false;
    } else if(state & MMAP){
        out << "cannot free buffers while mapped" << endl;
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
        out << "buffers array already created" << endl;
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
    out << "query all buffers" << endl;
    uint dMask = OPEN | REQBUFS | BUFFERS_ARRAY;
    if((state & dMask) != dMask){
        out << "depends OPEN & REQBUFS & BUFFERS_ARRAY" << endl;
        return false;
    }else if(state & QUERYBUF){
        // can't double queryAll because it overwrites buffers array
        out << "already queried" << endl;
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
    out << "mmap" << endl;
    uint dMask = OPEN | REQBUFS | BUFFERS_ARRAY | QUERYBUF;
    uint cMask = MMAP | QBUF;
    if(! ((state & dMask) == dMask)){
        out << "depends OPEN REQBUFS BUFFERS_ARRAY QUERYBUF" << endl;
        return false;
    } else if(state & cMask){
        out << "MMAP already done" << endl;
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
    out << "qbuf" << endl;
    uint dMask = OPEN | BUFFERS_ARRAY | MMAP;
    if(! ((state & dMask) == dMask)){
        out << "depends OPEN BUFFERS_ARRAY MMAP" << endl;
        return false;
    } else if(state & STREAM){
        out << "Don't do this while streaming is on" << endl;
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
    out << "stream" << endl;
    uint dMask = OPEN | MMAP | QBUF;
    if(! ((state & dMask) == dMask)){
        out << "depends OPEN MMAP QBUF" << endl;
        return false;
    } else if(state & STREAM){
        out << "Already Streaming" << endl;
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
        out << "Already ticking" << endl;
        return false;
    }
    timer->start(1);
    state = (State)(state | TIMER);
    return true;
}
bool CameraDataFeed::stopClock(){
    uint dMask = TIMER;
    if(! ((state & dMask) == dMask)){
        out << "Wasn't ticking already" << endl;
        return false;
    }
    timer->stop();
    state = (State)(state & ~TIMER);
    return true;
}
bool CameraDataFeed::stopStream(){
    out << "stream off" << endl;
    uint dMask = OPEN | STREAM;
    if(! ((state & dMask) == dMask)){
        out << "depends OPEN STREAM" << endl;
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
    out << "free mmap" << endl;
    uint dMask = OPEN | BUFFERS_ARRAY | MMAP;
    if(! ((state & dMask) == dMask)){
        out << "depends OPEN BUFFERS_ARRAY MMAP" << endl;
        return false;
    } else if(state & STREAM){
        out << "Cannot free mmap while streaming" << endl;
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
        out << "depends OPEN BUFFERS_ARRAY" << endl;
        return false;
    } else if(state & cMask){
        out << "cannot free buffer array while MMAP" << endl;
        return false;
    }
    delete[] buffers;
    state = (State)(state & ~(BUFFERS_ARRAY | QBUF | QUERYBUF));
    return true;
}
bool CameraDataFeed::closeCamera(){
    out << "closing camera" << endl;
    uint dMask = OPEN;
    uint cMask = MMAP;
    if(! ((state & dMask) == dMask)){
        out << "file not open" << endl;
        return false;
    } else if(state & cMask){
        out << "cannot close program while MMAP" << endl;
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
       } else if(errno == ENODEV){
           stopClock();
       } else {
           perror("readbuf");
           out << "err!" << endl;
       }
    } else {
        // VIDIOC_DQBUF doesn't actually return pointer to data when using mmap
        // Structure has index that can be used to look up pointer
        void * data = (void *)buffers[dqbuf.index].m.userptr;
        // Do something with data
//        emit newData(data);
        createImages(data);

        // tell driver it can reuse framebuffer
        ioctl(fd, VIDIOC_QBUF, &dqbuf);
        // signal repaint();
    }
}

void CameraDataFeed::createImages(void * voidData){
    const int width = v4l2Format.fmt.pix.width;
    const int height = v4l2Format.fmt.pix.height;
    const u_int32_t pixelFormat = v4l2Format.fmt.pix.pixelformat;
    u_int8_t * data = (u_int8_t *)voidData;

    QDateTime local(QDateTime::currentDateTime());
    int timestamp = (int)local.toUTC().toTime_t();
    QString common_filename = QString::number(timestamp) + local.toString("zzz")
            + QString(".png");
    vector<int> png_settings;
    png_settings.push_back(CV_IMWRITE_PNG_COMPRESSION);
    png_settings.push_back(0);

    Mat color_cv(height,width,CV_8UC2);
    Mat color_cv_rgb;
    Mat depth_cv(height,width, CV_16U);
    Mat ir_cv(height,width, CV_8U);
    Mat depth_cv_8;
    Mat depth_cv_rgb(height,width,CV_8UC3);
    Mat ir_cv_rgb(height,width,CV_8UC3);

    switch(pixelFormat){
    case YUYV:
        for(int j = 0 ; j < height ; j++){
            int step16 = width*2*j;
            for(int i = 0 ; i < width ; i++){
                int pixel16 = step16 + 2*i;

                u_int16_t color_yuyv = *(u_int16_t *)(data + pixel16);
                u_int8_t high = (color_yuyv >> 8) & 0xff;
                u_int8_t low = color_yuyv & 0xff;
                Vec2b colorpix_cv;
                colorpix_cv[1] = high;
                colorpix_cv[0] = low;
                color_cv.at<cv::Vec2b>(j,i) = colorpix_cv;
            }
        }
        cvtColor(color_cv,color_cv_rgb,CV_YUV2RGB_YUYV);
        colorImage = QImage(color_cv_rgb.data,color_cv_rgb.cols,color_cv_rgb.rows,
                            color_cv_rgb.step,QImage::Format_RGB888).copy();
        emit newColorImage(colorImage);
        if(takeSnap){
            Mat color_cv_bgr;
            cvtColor(color_cv_rgb, color_cv_bgr, CV_RGB2BGR);
            QString color_filename = snapshotDir + QString("/color/")
                    + common_filename;
            out << color_filename << endl;
            try {
                imwrite(color_filename.toStdString(), color_cv_bgr, png_settings);
            } catch (cv::Exception& error) {
                QTextStream(stderr) << "error writing image" << error.what() << endl;
            }
            takeSnap = false;
        }
        break;
    case INVZ:
    case INVR:
        // process depth
        break;
    case INVI:
    case RELI:
        // process infrared
        break;
    case INZI:
    case INRI:
        //process depth/ir
        for(int j = 0 ; j < height ; j++){
            int step24 = width*3*j;
            for(int i = 0 ; i < width ; i++){
                int pixel24 = step24 + 3*i;
                u_int16_t depth = *(u_int16_t *)(data + pixel24);
                u_int8_t ir = data[pixel24 + 2];
                depth = int(depth/31.25 + 0.5); // convert to mm
                u_int8_t high = (depth >> 8) & 0xff;
                u_int8_t low = depth & 0xff;
                Vec2b depthpix_cv;
                depthpix_cv[0] = high;
                depthpix_cv[1] = low;
                depth_cv.at<cv::Vec2b>(j,i) = depthpix_cv;
                ir_cv.at<uchar>(j,i) = ir;
            }
        }
        depth_cv.convertTo(depth_cv_8,CV_8U,1.0/256.0);
        cvtColor(depth_cv_8,depth_cv_rgb,CV_GRAY2RGB);
        depthImage = QImage(depth_cv_rgb.data,depth_cv_rgb.cols,depth_cv_rgb.rows,
                            depth_cv_rgb.step,QImage::Format_RGB888).copy();
        emit newDepthImage(depthImage);
        cvtColor(ir_cv,ir_cv_rgb,CV_GRAY2RGB);
        infraredImage = QImage(ir_cv_rgb.data,ir_cv_rgb.cols,ir_cv_rgb.rows,
                            ir_cv_rgb.step,QImage::Format_RGB888).copy();
        emit newInfraredImage(infraredImage);
        if(takeSnap){
            QString depth_filename = snapshotDir + QString("/depth/")
                    + common_filename;
            out << depth_filename << endl;
            try {
                imwrite(depth_filename.toStdString(), depth_cv, png_settings);
            } catch (cv::Exception& error) {
                QTextStream(stderr) << "error writing image" << error.what() << endl;
            }
            QString ir_filename = snapshotDir + QString("/ir/")
                    + common_filename;
            out << ir_filename << endl;
            try {
                imwrite(ir_filename.toStdString(), ir_cv, png_settings);
            } catch (cv::Exception& error) {
                QTextStream(stderr) << "error writing image" << error.what() << endl;
            }
            takeSnap = false;
        }
        break;
    }

/*
//            QRgb dPix = qRgb(low,
                             // invert green unless value is zero
//                             255 - (high == 0 ? 255 : high),
                             // depth below min become blue
//                             ((depth < depthMin) && (depth != 0)) ? 255 : 0);
//            QRgb irPix = qRgb(ir, ir, ir);
//              QRgb colorPix = qRgb(low, high, 0);
*/
}
void CameraDataFeed::setFourcc(u_int32_t fcc){
    fourcc = fcc;
}
void CameraDataFeed::setDepthMin(int minimum){
    out << "set depth min: " << minimum << endl;
    depthMin = (u_int16_t)minimum;
}
void CameraDataFeed::setDepthMax(int maximum){
    out << "set depth max: " << maximum << endl;
    depthMax = (u_int16_t)maximum;
}
void CameraDataFeed::setDepthMask(int byteMask){
    out << "set depth mask" << endl;
    depthMask = (u_int16_t)byteMask;
}

void CameraDataFeed::savePicture(){
    takeSnap = true;
}

void CameraDataFeed::setSnapshotDir(QString dir){
    snapshotDir = dir;
}

void CameraDataFeed::setCameraDevice(QString dev){
    device = dev;
}
