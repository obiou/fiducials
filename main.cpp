/**
 * @author  Steven Lovegrove
 * Copyright (C) 2010  Steven Lovegrove
 *                     Imperial College London
 **/

#include <pangolin/pangolin.h>
#include <pangolin/video.h>

using namespace pangolin;

int main( int /*argc*/, char* argv[] )
{
  std::string video_uri = "v4l:///dev/video0";

  // Setup Firewire Camera
//  FirewireVideo video(0,DC1394_VIDEO_MODE_640x480_RGB8,DC1394_FRAMERATE_30,DC1394_ISO_SPEED_400,50);
  VideoInput video(video_uri);

  const unsigned w = video.Width();
  const unsigned h = video.Height();

  // Create Glut window
  pangolin::CreateGlutWindowAndBind("Main",w,h);

  // Create viewport for video with fixed aspect
  View& vVideo = Display("Video").SetAspect((float)w/h);

  // OpenGl Texture for video frame
  GlTexture texVideo(w,h,GL_RGBA8);

  unsigned char* rgb = new unsigned char[video.Width()*video.Height()*3];

  for(int frame=0; !pangolin::ShouldQuit(); ++frame)
  {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // Get newest frame from camera and upload to GPU as texture
    video.GrabNext(rgb,true);
    texVideo.Upload(rgb,GL_RGB,GL_UNSIGNED_BYTE);

    // Activate video viewport and render texture
    vVideo.Activate();
    texVideo.RenderToViewportFlipY();

    // Swap back buffer with front
    glutSwapBuffers();

    // Process window events via GLUT
    glutMainLoopEvent();
  }

  delete[] rgb;

  return 0;
}
