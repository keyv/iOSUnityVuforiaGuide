# iOSUnityVuforiaGuide
Integration of Unity 5.3 + Vuforia 5.5.9 project with native iOS application (Xcode 7.3)

Inspired by [https://the-nerd.be/](www.the-nerd.be) tutorials, by Frederik Jacques.

Let's assume that you already have a Unity project which uses Vuforia SDK.

In project's Player Settings be sure that you use:
* Scripting Backend - IL2CPP;
* Uncheck 'Auto Graphics API' and use only OpenGLES2, because for iOS 9+ Unity tries to use Metal and you can face some issues with that (in my case there were no texture on 3d object).

Add scene to build and generate iOS project from Unity project and after this step you can close Unity.

##Step 1
Create Xcode project and add `UnityIntegration.xcconfig` which you can find in this repo.

As soon as we need to use files from previously generated iOS project, you can put the projects together in a common directory.

Select project settings and choose that file to be used by your project:

![](imgs/1.png?raw=true "")

Now choose `Target -> Build Settings` scroll down to the end of the list and change values of 
`UNITY_IOS_EXPORTED_PATH` and `UNITY_RUNTIME_VERSION` to path of Unity-generated iOS project and version of Unity you use accordingly.

![](imgs/2.png?raw=true "")

##Step 2
Remove `Main.storyboard` from Xcode project.
Also, in `Info.plist` remove `Main storyboard file base name` row:

![](imgs/3.png?raw=true "")

##Step 4
Create new group, let's call it "Integration".
Drag and drop inside thisgroup `Classes` and `Libraries` folders from previously Unity-generated iOS.
Make sure, that "Copy resources if needed" is <em>unchecked</em>. This could take few minutes, as soon as there's a lot of files inside the folders.
![](imgs/4.png?raw=true "")

##Step 4

Add frameworks to the project, so the list will look like this:
![](imgs/5.png?raw=true "")


