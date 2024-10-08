# Overview
This project is for a user interface that works along side a Mobile Tracking System developed for a Raspberry pi that is mounted to a drone. You can access that project [here](https://github.com/csaethre19/MobileTargetTracking). 

# Usage
- This application needs the [SoloGood FPV Receiver](https://www.amazon.com/gp/product/B08YJGCVJS/ref=ox_sc_act_image_2?smid=A2XZ0PQGR3TYBH&psc=1) to receive video from the drone. This is connected to the computer running the application via USB.
- This application also requires the [Nova swarm dongle ground station](https://redmoredynamics.com/) that is connected to the users.
- To run the application you can navigate to the [release section on GitHub](https://github.com/KirraKotsenburg/TrackerApp/releases) and download the zip file, extract, and run the appTrackerApp.exe
- When you launch the program you can select the camera port and the COM port. 
- <img src="images/mts-home.png" width="250" height="300" /> <br>
      - When the receivers index is selected for camera port you should see a static screen. <br>
      - To determine which COM device you should select go to your computers Device Manager and plug in the Nova swarm dongle to determine which COM name it comes up as and select that one in the app. 

- Once a successful UART connection is made and the video from the drone is being displayed within the user interface, initiate tracking by clicking the Start Tracking button and as instructed draw a box around an object within the frame. If tracking is successful a blue bounding box will stay around the specified object otherwise a Tracking Failed message will be displayed and the user can initiate tracking again.


# Dev Set up
[Qt Creator Open Source download link](https://www.qt.io/download-qt-installer-oss?utm_referrer=https%3A%2F%2Fwww.qt.io%2Fdownload-open-source)

  After you have downloaded QT Creator, select the version you want installed in the QT Maintanence app
  **As of 6/12/24 we are using version 6.7.1

### Environment Variables Needed
- OPENCV_PATH
- QT_VERSION

Type env in your windows search bar and open "Edit system environment variables" <br>
Click on the "Environment Variables..." button <br>
Click "New" <br>
Add the variable name and your value e.g. OPENCV_PATH for variable name and C:/opencv/build for variable value

### OpenCv With QT Creator:

  Helpful Links
    [QT Creator wiki instructions](https://wiki.qt.io/How_to_setup_Qt_and_openCV_on_Windows)
  
   [Video with steps for set up](https://m.youtube.com/watch?v=0KNh_7fUqrM)
  
  Step 1: Install OpenCV -> https://sourceforge.net/projects/opencvlibrary/
  
  
  Step 2: In the Qt Maintainence app make sure that you have MinGW (version) 32-bit or 64-bit intstalled depending on your computer.
        Then add Qt minGW to the windows path variable. The path added should look something like this - 
        ![image](https://github.com/KirraKotsenburg/TrackerApp/assets/90931675/2d6ba2d1-ba5b-4674-af25-a97afd2553a0)
        
  
  Step 3: Download CMake if you don't already have it -> https://cmake.org/download/
  

  Step 4: Open the CMake GUI and select the OpenCV Source and Build directories then click configure-
  
  ![image](https://github.com/KirraKotsenburg/TrackerApp/assets/90931675/62f94304-2eac-4b6a-9ef4-3aa2fb2ecde6)
  

  Step 5: There should be a pop up window after you click configure, in it you want to specify generators as "MinGW Makefiles" and then
        choose "Specify native compilers" before clicking next.


  Step 6: Next you will want to select the compilers to build OpenCV - 
  
  ![image](https://github.com/KirraKotsenburg/TrackerApp/assets/90931675/b227f654-edf8-4493-894e-96960ef33cc3)
  
  Once it has finished building click finish.


  Step 7: You will need to use the search bar in the GUI and look up: WITH_QT and WITH_OPENGL and check the boxes next to the entries.


  Step 8: Click "Add Entry" button and fill in spaces like so -

  ![image](https://github.com/KirraKotsenburg/TrackerApp/assets/90931675/ffac9abd-948f-40b7-9804-5c6f22a9ca1b)


  Step 9:Click "Add Entry" button again and fill in the spaces like so to enter the PATH to QMake executable- 

  ![image](https://github.com/KirraKotsenburg/TrackerApp/assets/90931675/1f728222-9b4d-456e-aa4f-9338178c8542)


  Step 10: Click "Configure" and wait for CMake to configure OpenCV (everything in red has not been applied yet) - If
        nothing is red after the configuration then it was a success. (You may have to click "Configure" twice to get rid of red)


  Step 11: Verify that the following settings are shown like so -

  ![image](https://github.com/KirraKotsenburg/TrackerApp/assets/90931675/5c362af0-bc03-44a3-b405-f2fd960bb20d)


  Step 12: Click "Generate" and wait for CMake to finish


  Step 13: Next you'll want to open a cmd window and cd into the OpenCV build file and enter these two commands -

  ![image](https://github.com/KirraKotsenburg/TrackerApp/assets/90931675/fb0727a6-fadd-4dae-a13a-5690bb1d095a)


  Step 14: Once the build is complete enter this command in the window to install OpenCV -

  ![image](https://github.com/KirraKotsenburg/TrackerApp/assets/90931675/761740dc-b5a8-49ed-acee-623e5e96f313)


  Step 15: Add OpenCv to Windows PATH variable, it should look something like this -

  ![image](https://github.com/KirraKotsenburg/TrackerApp/assets/90931675/dafd1d95-ac75-4411-a160-489d1918632c)


  Step 16: You're ready to use OpenCV and QT Creator together now!

  # Release Steps

  1. Build app in Qt Creator in Release mode
  2. Run in the directory of the release folder where .exe file is for app: C:\Qt\6.7.2\mingw_64\bin\windeployqt.exe appTrackerApp.exe
      - Path depends on where your Qt installation is located and what version, just find the windeployqt.exe
      - This copies over the Qt library dll's that are needed
3. Copy all contents of release folder (located in the build folder of project directory) plus all dlls for opencv (C:\opencv\build\bin or wherever your opencv installation is located) to another folder and zip it e.g. trackerApp-v1-release-win
4. Copy all dll's from C:\Qt\6.7.2\mingw_64\bin (Also depends on your installation of Qt)
5. Zip folder
6. Add a new release on GitHub and add this zip
