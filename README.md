# Putty_With_Serial_Delay

This project implements a simple add-on feature to PuTTy for serial communications. On a Windows based system, this version of Putty allows the user to specify a character and line delay for the serial protocol terminal communication. This can come in handy when send large amounts of data to a serial device that is unable to keep up with the native transfer speed of Putty. One example of this are Cisco switches and routers. For example, when attempting to load a config file through the serial console, errors will sometimes be created if the config is large enough as some characters will be missed.

I prefer Putty over other options I have tried and really wanted this feature for those occassional times I need it. I saw it has been on the wishlist for Putty for awhile, but the developers have never added it. This isn't a very high demand feature I would imagine so not getting around to this feature is understandable. The delays are implemented in milliseconds and do not have a restriction on how high you set the time (expect strange behavior if you set the delay time really high).

I have named the exeuctable "PuttyES_071" because I used version 71 of Putty to add this feature. the "ES" stands for "extended serial." It should be possible to use this version of Putty alongside a standard version without any issues. I believe they will share the same settings file which is stored in the registry I believe (on Windows anyway), but in my limited testing I haven't had any problems using eithe or both.

## Prerequisites 

There are no requirements to run this version of Putty beyond those of the standard version. The add-on features will only work under Windows based systems however.

## Getting Started / Installation

The self-executable can be found in the build folder; no installation necessary. There is nothing you need to do to start using this version of Putty other than to run the exe. Once Putty starts you can configure your serial delay options by going to Connection -> Serial. On the options on the right side the delay options have been added to the bottom of the settings. Character and line delay are both in milliseconds. Because these settings are integrated into the configuration of Putty, they will also be saved if you create a saved session with all of your serial connection settings for later use.

Another addition I have implemented is a helper window that will come up with the serial terminal once a connection has been established. The character and line delays can also be changed from here on the fly, but it is not possible to save the values from the sub-window like on the options settings prior to connection. It is possible to close the sub-window if it is getting in the way without closing Putty. The last settings you entered into the sub-window will be retained when you close the window. 

I have included a couple screenshots in the docs folder showing where the delay settings have been added. 

## Testing

This software is presented as is, with no guarantee of performance. I made a best effort attempt to add the serial delay feature in keeping with the philosophy of the original software (that is, using only standard C code). I did not have convenient access to other platforms, and so I have only implemented the feature under Windows. I discovered while digging around that there are really no great time delay options when straight standard C under Windows. There are some updated options such as [queryperformancetimer](https://docs.microsoft.com/en-us/windows/win32/sysinfo/acquiring-high-resolution-time-stamps) that can be used, but they required C++ or using a different include than that which is already being used in PuTTy (and they did not play nice together). In the end I just went with a basic delay loop that should be more than sufficient for the intended purpose. If you are doing something with a serial connection that needs high precision timing however then this serial delay feature is probably not for you.

I also made a best effort to test the delay to ensure the expected behavior, but I cannot say that it is error free. I am not terribly experienced with C, and this was my first foray into the world of pure C based GUI. Most of my efforts went into sorting out what was going on there just so I could get the feature to work at all (hwnd, you are now the stuff of my nightmares). To add to the challenge the problem that I was trying to solve, data loss between putty and a serial device due to transfer speed, is inconsistent. I most often use a serial connection with Putty to upload config files to Cisco routers and switches like the 2960. If the config is small enough, or because the stars align, it may be possible to load a config without issues. Even the same config with the same router may load fine several times, but have a problem on the next transfer. Add to that the fact that I do no have consistent access to a serial device to test with and I had to reach a point where I said it was good enough for a start.

If you do experienc issues with the delay not working as intended let me know. Explain what you were doing, if the error is reproducible, and if it's something I might be able to replicate I would also appreciate the details of your setup so that I can replicate it on my end. I can revisit this and try to resolve any issues that come up if you let me know what they are.

## About the Source Files

I have only included the source files I created myself or edited from the existing Putty source code. Since Putty is fully open source the whole codebase can be downlaoded directly from [the Putty website](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html). There are a lot of files and, even if the license allows permission to share the whole thing, it seemed like it would be less confusing to simply provide what was different. If you would like to enhance this version further, I would recommend downloading the source code directly from the Putty website first. Once you have the source code setup in your favorite IDE you can switch out my edited files in your project and you're ready to continue. It makes the source code here much smaller and hopefully will allow those who want to work with my code to do so with the most current version of Putty at the time. I would recommend verifying differences in the files I have provided against the official Putty version you download however before overwriting them to be sure that a new feature or important piece of new code isn't lost.

## Version History

* **PuttyES v071** - Extended serial delay feature added to PuTTy version 071.

## Author(s)

Putty:
**Simon Tatham**

Serial delay add on: 
**Jeremy Ruth**

## License

This project is licensed under the MIT License license. See [License](https://github.com/Jeremy-Ruth/Putty_With_Serial_Delay/blob/master/LICENSE) for more details.

