## QtPngWarning

某些格式的png图标, 在Qt程序加载使用的时候会输出日志:`libpng warning: iCCP: known incorrect sRGB profile.` 虽然使用毫无问题. 但为了日志的整洁, 希望解决这个问题.

	解决方法是, 用QImage加载原png图片, 再原封不动写出来. 这相当于是给png做了一个Qt的适配. 

于是有了这个小程序.

##### 预览:

![alt text](./preview.png "")
