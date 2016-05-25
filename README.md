# iOSUnityVuforiaGuide
Integration of Unity 5.3 + Vuforia 5.5.9 project with native iOS application (Xcode 7.3)

Inspired by [the-nerd.be](https://the-nerd.be/) tutorials, by Frederik Jacques.

Let's assume that you already have a Unity project which uses Vuforia SDK.

In project's Player Settings be sure that you use:
* Scripting Backend - IL2CPP
* Uncheck 'Auto Graphics API' and use only OpenGLES2, because for iOS 9+ Unity tries to use Metal and you can face some issues with that (in my case there were no texture on 3d object)

Add scene to build and generate iOS project from Unity project and after this step you can close Unity.

##Step 1
Create Xcode project and add `UnityIntegration.xcconfig` which you can find in this repo.

As soon as we need to use files from previously Unity-generated iOS project, you can put the projects together in a common directory. Do not delete project generated with Unity even after all steps done. 

Select project's settings and choose `UnityIntegration.xcconfig` file to be used by your project:

![](imgs/1_1.png?raw=true "")

Now choose `Target -> Build Settings`, scroll down to the end of the list and change values of 
`UNITY_IOS_EXPORTED_PATH` and `UNITY_RUNTIME_VERSION` to path of Unity-generated iOS project and version of Unity you use accordingly.

![](imgs/1_2.png?raw=true "")

##Step 2
Remove `Main.storyboard` from Xcode project.
Also, in `Info.plist` remove `Main storyboard file base name` row:

![](imgs/2.png?raw=true "")

##Step 3
Create new group, let's call it `"Integration"`.

Drag and drop inside this group `Classes` and `Libraries` folders from previously Unity-generated iOS project.

Make sure, that:
* `Copy resources if needed` is <strong>unchecked</strong>
* `Create groups` is <strong>checked</strong>

This operation could take few minutes as soon as there's a lot of files inside the folders.

![](imgs/3.png?raw=true "")

##Step 4

Now let's make some cleanup, so Xcode won't be lagging while processing a lot of not neeed files.

###Step 4.1
Inside `Classes` folder choose `Native` folder and search for `.h` files on the bottom of Navigator.

We need to remove references to `.h` files inside `Native` folder, except those which starts with `"Vuforia"` (they're at the end of the list). I'd recommend not to select and remove all files at once, because Xcode can stuck or even crash :( 

So when pressing `Delete`, make sure to press <em>`Remove References`</em>:

![](imgs/4_1.png?raw=true "")

###Step 4.2

Go to `Libraries` folder and remove reference for `libil2cpp` folder.

Again, make sure to press <em>`Remove References`</em>:

![](imgs/4_2.png?raw=true "")

##Step 5
Also we need to add some more files from previously Unity-generated iOS project.

* Drag and drop inside the `Integration` group `Data` folder
* Drag and drop inside the `Integration` group `QCAR` folder wich is in `Data/Raw/` folder

Check if:
* `Copy resources if needed` is <strong>unchecked</strong>
* `Create folder references` is <strong>checked</strong>

![](imgs/5_1.png?raw=true "")

So you will have something like this:

![](imgs/5_2.png?raw=true "")


##Step 6

Add frameworks to the project, so the list will look like this:

![](imgs/6.png?raw=true "")

##Step 7

Create `PrefixHeader.pch` file: `File -> New -> Other -> PCH file`

Remove all code inside and insert the following: 

```
#ifndef PrefixHeader_pch
#define PrefixHeader_pch

#ifdef __OBJC__
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#endif

#include "Preprocessor.h"
#include "UnityTrampolineConfigure.h"
#include "UnityInterface.h"

#ifndef __OBJC__
#if USE_IL2CPP_PCH
#include "il2cpp_precompiled_header.h"
#endif
#endif

#ifndef TARGET_IPHONE_SIMULATOR
#define TARGET_IPHONE_SIMULATOR 0
#endif

#define printf_console printf

#endif
```

Now in the `Build Settings` under `Apple LLVM 7.1 - Language` section, find the field called `Prefix Header` and instead of `/ENTER/PATH/HERE` add path to previously created file, e.g.: `YOUR_PROJECT_NAME/PrefixHeader.pch` which in my case is:  `UnityIntegration/PrefixHeader.pch`

![](imgs/7.png?raw=true "")

##Step 8

Rename `Supporting Files/main.m` to `main.mm` and `AppDelegate.m` to `AppDelegate.mm`

![](imgs/8.png?raw=true "")

##Step 9

Go to `Integration/Classes/main.mm`, copy all code from this file and paste it instead of code in `Supporting Files/main.mm`

Now, in `Supporting Files/main.mm` change line: `const char* AppControllerClassName = "UnityAppController";`

with: `const char* AppControllerClassName = "AppDelegate";`

##Step 10

Go to `Build Settings`, search for `"main"` and in `Compile Sources` section remove file which corresponds to `Classes` folder:

![](imgs/10.png?raw=true "")

##Step 11

Inside `UnityAppController.h` file make the following changes:

Comment part:

```
inline UnityAppController*	GetAppController()
{
	return (UnityAppController*)[UIApplication sharedApplication].delegate;
}

```

and paste instead:


```
NS_INLINE UnityAppController* GetAppController()
{
	NSObject<UIApplicationDelegate>* delegate = [UIApplication sharedApplication].delegate;
	UnityAppController* currentUnityController = (UnityAppController *)[delegate valueForKey:@"unityController"];
	return currentUnityController;
}
```

Inside `UnityAppController.mm` file make the following changes:

`#import "AppDelegate.h"`

Replace empty `- (void)shouldAttachRenderDelegate` method with: 

```
- (void)shouldAttachRenderDelegate	{

	AppDelegate *delegate = (AppDelegate *)[UIApplication sharedApplication].delegate;
	[delegate shouldAttachRenderDelegate];
	
}
```

##Step 12
Inside `VuforiaNativeRendererController.mm` file comment last line:
`IMPL_APP_CONTROLLER_SUBCLASS(VuforiaNativeRendererController)`

##Step 13
Let's create a button which will open Unity+Vuforia view inside our app.

In `ViewController.m` add the following parts:

`#import "AppDelegate.h"`

`@property (nonatomic, strong) UIButton *showUnityButton;`

```
- (void)viewDidLoad {
	[super viewDidLoad];
	self.view.backgroundColor = [UIColor blueColor];
	
	self.showUnityButton = [UIButton buttonWithType:UIButtonTypeSystem];
	[self.showUnityButton setTitle:@"SHOW UNITY" forState:UIControlStateNormal];
	self.showUnityButton.frame = CGRectMake(0, 0, 100, 44);
	self.showUnityButton.center = self.view.center;
	
	[self.view addSubview:self.showUnityButton];
	
	[self.showUnityButton addTarget:self action:@selector(showUnityButton:) forControlEvents:UIControlEventTouchUpInside];
	
}

- (void)showUnityButton: (UIButton *) sender {
	[(AppDelegate *)[UIApplication sharedApplication].delegate showUnityWindow];
}
```

##Step 14

In this repo you can find both `AppDelegate.h` and `AppDelegate.mm` files and paste their content into your files.
Shortly, what is going on in those files:

* We create 2 `UIWindow` instances for main and Unity content
* to switch between those windows we use `- (void)showUnityWindow` and  `- (void)hideUnityWindow` methods
* also we have instance `unityController` of `UnityAppController` type, because we took away control from Unity-generated app delegate and we need to pass calls to it through our app delegate
* in `(BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions ` we create those windows, root view controller, navigation controller etc. so app will work properly

#Final
Building and running the app will show something like this: we press `Show Unity` button, it opens Unity+Vuforia view which recognizes the marker well and shows nice yellow button in center, which can bring us back to previous view controller 


### Useful links

* [https://the-nerd.be/2015/11/13/integrate-unity-5-in-a-native-ios-app-with-xcode-7/](https://the-nerd.be/2015/11/13/integrate-unity-5-in-a-native-ios-app-with-xcode-7/)
* [http://www.makethegame.net/unity/add-unity3d-to-native-ios-app-with-unity-5-and-vuforia-4-x/](http://www.makethegame.net/unity/add-unity3d-to-native-ios-app-with-unity-5-and-vuforia-4-x/)
* [https://github.com/blitzagency/ios-unity5](https://github.com/blitzagency/ios-unity5)

