#ifndef templates_h
#define templates_h

/* http://meyerweb.com/eric/tools/css/reset/
   v2.0 | 20110126
   License: none (public domain)
*/
const String cssResetTemplate = R"HTML(
a,abbr,acronym,address,applet,article,aside,audio,b,big,blockquote,body,canvas,caption,center,cite,code,dd,del,details,dfn,div,dl,dt,em,embed,fieldset,figcaption,figure,footer,form,h1,h2,h3,h4,h5,h6,header,hgroup,html,i,iframe,img,ins,kbd,label,legend,li,mark,menu,nav,object,ol,output,p,pre,q,ruby,s,samp,section,small,span,strike,strong,sub,summary,sup,table,tbody,td,tfoot,th,thead,time,tr,tt,u,ul,var,video{margin:0;padding:0;border:0;font-size:100%;font:inherit;vertical-align:baseline}article,aside,details,figcaption,figure,footer,header,hgroup,menu,nav,section{display:block}body{line-height:1}ol,ul{list-style:none}blockquote,q{quotes:none}blockquote:after,blockquote:before,q:after,q:before{content:'';content:none}table{border-collapse:collapse;border-spacing:0}
)HTML";

const String stylesTemplate = R"HTML(
h1{font-size:xx-large;font-weight:bolder;margin-bottom:1.5em}label{font-weight:700}body{padding:1em}section{margin-bottom:.5em}
)HTML";

const String pageTemplate = R"HTML(
<!DOCTYPE HTML>
<html>
<head>
    <style>{{styles}}</style>
</head>
<body>
{{content}}
</body>
</html>
)HTML";

const String infoPageTemplate = R"HTML(
<h1>Device info</h1>
<section>
    <div><label>RAM free: </label><span>{{ramFree}}b</span></div>
    <div><label>RAM used: </label><span>{{ramUsed}}b ({{ramUsedPercent}}%)</span></div>
</section>
<section>
    <div><label>App version: </label><span>{{appVersion}}</span></div>
    <div><label>Arduino version: </label><span>{{arduinoVersion}}</span></div>
</section>
)HTML";

#endif
