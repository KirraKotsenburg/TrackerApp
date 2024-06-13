# TrackerApp
Mobile Tracking project app for user

Qt Creator Open Source download link:
  https://www.qt.io/download-qt-installer-oss?utm_referrer=https%3A%2F%2Fwww.qt.io%2Fdownload-open-source

  After you have downloaded QT Creator, select the version you want installed in the QT Maintanence app
  **As of 6/12/24 we are using version 6.7.1

OpenCv With QT Creator:

  Helpful Links
    QT Creator wiki instructions:
      https://wiki.qt.io/How_to_setup_Qt_and_openCV_on_Windows
  
    Video with steps for set up:
      https://m.youtube.com/watch?v=0KNh_7fUqrM
  
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
