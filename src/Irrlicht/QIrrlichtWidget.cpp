
// QIrrlichtWidget.cpp
// http://irrlicht.sourceforge.net/forum/viewtopic.php?f=5&t=44658

#include <iostream>
 
#include <QtCore/QDebug>


#include "QIrrlichtWidget.hpp" 

#include "stereo.hpp"

QIrrlichtWidget::QIrrlichtWidget(QWidget *parent) :
  QWidget(parent)
{
    // Indicates that the widget wants to draw directly onto the screen. (From documentation : http://doc.qt.nokia.com/latest/qt.html)
    // Essential to have this or there will be nothing displayed
    //setAttribute(Qt::WA_PaintOnScreen);

    // Indicates that the widget paints all its pixels when it receives a paint event.
    // Thus, it is not required for operations like updating, resizing, scrolling and focus changes to erase the widget before generating paint events.
    // Not sure this is required for the program to run properly, but it is here just incase.
    setAttribute(Qt::WA_OpaquePaintEvent);

    // Widget accepts focus by both tabbing and clicking
    setFocusPolicy(Qt::StrongFocus);

    // Not sure if this is necessary, but it was in the code I am basing this solution off of
    setAutoFillBackground(false);
 
    m_device = 0;
}

QIrrlichtWidget::~QIrrlichtWidget()
{
    if (m_device != 0)
    {
        m_device->closeDevice();
        m_device->drop();
    }
}
 
// Create the Irrlicht device and connect the signals and slots
void QIrrlichtWidget::init()
{
    // Make sure we can't create the device twice
    if(m_device != 0)
        return;
 
    // Set all the device creation parameters
    SIrrlichtCreationParameters params;
    params.AntiAlias = 0;
    params.Bits = 32; // 32
    params.DeviceType = EIDT_X11; // EIDT_X11 EIDT_BEST
    params.Doublebuffer = true;
    params.DriverType = video::EDT_OPENGL; // EDT_OPENGL
    params.EventReceiver = 0;
    params.Fullscreen = false;
    params.HighPrecisionFPU = false;
    params.IgnoreInput = false;
    params.LoggingLevel = ELL_INFORMATION;
    params.Stencilbuffer = true; // true
    params.Stereobuffer = false;
    params.Vsync = false;
    // Specify which window/widget to render to
    params.WindowId = reinterpret_cast<void*>(winId());
    params.WindowSize.Width = width();
    params.WindowSize.Height = height();
    params.WithAlphaChannel = false;
    params.ZBufferBits = 16;
 
    // Create the Irrlicht Device with the previously specified parameters
    m_device = createDeviceEx(params);
 
    if(m_device) {
      // Create a camera so we can view the scene
      //m_camera = m_device->getSceneManager()->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));
      m_camera = m_device->getSceneManager()->addCameraSceneNode(0,                       // Parent
                                                                 vector3df(0, 0, -10),    // Position
                                                                 vector3df(0, 0, 0));     // Lookat
      m_camera->setNearValue (10.0f) ; // default 1.0f
      m_camera->setFarValue (3000.f) ; // default 3000.0f
      m_camera->setFOV (PI / 2.5f) ;   // default PI/2.5f = 1.25663706144 (Field Of View in radians)

      printCameraInfo () ;

    }
 
    // Connect the update signal (updateIrrlichtQuery) to the update slot (updateIrrlicht)
    connect(this, SIGNAL(updateIrrlichtQuery(IrrlichtDevice*)), this, SLOT(updateIrrlicht(IrrlichtDevice*)));
 
    // Start a timer. A timer with setting 0 will update as often as possible.
    startTimer(5);
}

void QIrrlichtWidget::printCameraInfo ()
{
  if (m_camera) {
    core::vector3df position = m_camera->getPosition () ;
    core::vector3df lookat = m_camera->getTarget () ;
    const SViewFrustum* frustum = m_camera->getViewFrustum() ;

    std::cout << "CAMERA" << std::endl ;
    std::cout << "  Position     : (" << position.X << ", " << position.Y << ", "  << position.Z << ")" << std::endl ;
    std::cout << "  Lookat       : (" << lookat.X << ", " << lookat.Y << ", "  << lookat.Z << ")" << std::endl ;
    std::cout << "  Aspect Ratio : " << m_camera->getAspectRatio () << std::endl ;
    std::cout << "  FOV          : " << m_camera->getFOV () << " rad" << std::endl ;
    std::cout << "  Near Value   : " << m_camera->getNearValue () << std::endl ;
    std::cout << "  Far Value    : " << m_camera->getFarValue () << std::endl ;
    std::cout << "  Frustum      : Near Z = " << frustum->getNearLeftDown().X << " Far Z = " << frustum->getFarLeftDown().X << std::endl ;
    std::cout << std::endl ;
  }
}

IrrlichtDevice* QIrrlichtWidget::getIrrlichtDevice()
{
    return m_device;
}
 
void QIrrlichtWidget::paintEvent(QPaintEvent* event)
{
    event = event ; // To avoid warning "not used"

    if(m_device != 0)
    {
        emit updateIrrlichtQuery(m_device);
    }
}
 
void QIrrlichtWidget::timerEvent(QTimerEvent* event)
{
    // Emit the render signal each time the timer goes off
    if (m_device != 0)
    {
        emit updateIrrlichtQuery(m_device);
    }
 
    event->accept();
}
 
void QIrrlichtWidget::resizeEvent(QResizeEvent* event)
{
  std::cerr << "QIrrlichtWidget::resizeEvent()" << std::endl ;

    if(m_device != 0)
    {
        dimension2d<u32> widgetSize;
        widgetSize.Width = event->size().width();
        widgetSize.Height = event->size().height();
        m_device->getVideoDriver()->OnResize(widgetSize);
 
        ICameraSceneNode *cam = m_device->getSceneManager()->getActiveCamera();
        if (cam != 0)
        {
          cam->setAspectRatio((f32)widgetSize.Height / (f32)widgetSize.Width);
        }
    }
 
    QWidget::resizeEvent(event);
}
 
void QIrrlichtWidget::updateIrrlicht( irr::IrrlichtDevice* device )
{
    if(device != 0)
    {
        device->getTimer()->tick();

        /* 
        SColor color (255,100,100,140);
        device->getVideoDriver()->beginScene(true, true, color);
        device->getSceneManager()->drawAll();
        //if (m_bStereo) {
        //  device->getVideoDriver()->setRenderTarget (video::ERT_STEREO_BOTH_BUFFERS, true, true, video::SColor(0, 0, 0, 1)) ;
        //
        //  device->getVideoDriver()->setRenderTarget (video::ERT_STEREO_LEFT_BUFFER, true, true, video::SColor(0, 0, 0, 1)) ;
        //  device->getSceneManager()->setActiveCamera (m_camLeft) ;
        //  device->getSceneManager()->drawAll () ;
        //
        //  device->getVideoDriver()->setRenderTarget (video::ERT_STEREO_RIGHT_BUFFER, true, true, video::SColor(0, 0, 0, 1)) ;
        //  device->getSceneManager()->setActiveCamera (m_camRight) ;
        //  device->getSceneManager()->drawAll () ;
        //  //}
        device->getVideoDriver()->endScene();
        */

        /*
      DrawAnaglyph(
                   device->getVideoDriver(), device->getSceneManager(), device->getSceneManager()->getActiveCamera(),
                   video::SColor(0,200,200,255), // Background color (255,200,200,200)
                   2, // fWidth
                   1000, // fFocus
                   video::EDT_OPENGL,
                   0x000000ff,
                   0x00ffff00
                   );
        */

      //std::cout << "QIrrlichtWidget::updateIrrlicht(device)" << std::endl ;
    }
 
}

void QIrrlichtWidget::updateIrrlicht()
{
    if(m_device != 0)
    {
        m_device->getTimer()->tick();
 
        /*
        SColor color (255,100,100,140); 
        m_device->getVideoDriver()->beginScene(true, true, color);
        m_device->getSceneManager()->drawAll();
        m_device->getVideoDriver()->endScene();
        */

        /*
      DrawAnaglyph(
                   m_device->getVideoDriver(), m_device->getSceneManager(), m_device->getSceneManager()->getActiveCamera(),
                   video::SColor(0,200,200,255), // Background color (255,200,200,200)
                   2, // fWidth
                   1000, // fFocus
                   video::EDT_OPENGL,
                   0x000000ff,
                   0x00ffff00
                   );
        */

      //std::cout << "QIrrlichtWidget::updateIrrlicht()" << std::endl ;

    }
 
}
 
// Include the extra Qt file for signals and slots
#include "moc_QIrrlichtWidget.cpp"

