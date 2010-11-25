<<<<<<< HEAD
// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

var downloader = "";
var contextMenuStatus = false;
var flashgetStatus = false;
var miniFlashgetStatus = false;
var thunderStatus = false;
var miniThunderStatus = false;
var useExperimentalAPI;

chrome.extension.onRequest.addListener(
    function(request, sender, sendResponse) {
      if (request && request.msg == 'init_check') {
        downloader = request.downloader;
        contextMenuStatus = eval(request.contextmenu);
        flashgetStatus = eval(request.flashget);
        miniFlashgetStatus = eval(request.miniFlashget);
        thunderStatus = eval(request.thunder);
        miniThunderStatus = eval(request.miniThunder);
        sendResponse({msg: 'is_content_script'});
      } else if (request.msg == 'content_script_is_load') {
        sendResponse(checkLinks());
      }
});

function generateContextMenu() {
  var contextMenu = document.createElement('div');
  contextMenu.id = 'dh-menu';
  contextMenu.style.display = 'none';
  var menuList = ['menu_flashget', 'menu_mini_flashget', 'menu_thunder',
      'menu_mini_thunder', 'menu_chrome', 'menu_copy_link', '', 'menu_disable'];
  for (var i = 0; i < menuList.length; i++) {
    var menuItem = document.createElement('div');
    if (menuList[i] == '') {
      menuItem.className = 'dh-menu-line';
    } else {
      menuItem.className = 'dh-menu-item';
      var a = document.createElement('a');
      a.id = menuList[i];
      a.href = 'javascript:void(0)';
      a.innerText = chrome.i18n.getMessage(a.id);
      menuItem.appendChild(a);
    }
    contextMenu.appendChild(menuItem);
  }
  document.body.appendChild(contextMenu);

  document.onclick = function() {
    if (document.getElementById('dh-menu')) {
      contextMenu.parentNode.removeChild(contextMenu);
      console.log("remove");
    }
  }
}

function loadContextMenuCss() {
  var cssRef = document.createElement('link');
  cssRef.id = 'drag-style';
  cssRef.rel = 'stylesheet';
  cssRef.href = chrome.extension.getURL('style.css');
  cssRef.type = 'text/css';
  document.getElementsByTagName('head')[0].appendChild(cssRef);
}


function init() {
  loadContextMenuCss();
  
  chrome.extension.sendRequest({command: 'isUseExperimentalAPI'},
    function(response) {
      useExperimentalAPI = response.useExperimentalAPI;
      if (!useExperimentalAPI) {
        chrome.extension.sendRequest(
          {command: 'update'}, function(response) {
            downloader = response.downloader;
            contextMenuStatus = eval(response.contextmenu);
            flashgetStatus = eval(response.flashget);
            miniFlashgetStatus = eval(response.miniFlashget);
            thunderStatus = eval(response.thunder);
           // webThunderStatus = eval(response.webThunder);
            miniThunderStatus = eval(response.miniThunder);});
      }

      var sufix = /(\.CRX|\.ZIP|\.EXE|\.BIN|\.GZ|\.Z|\.TAR|\.ARJ|\.LZH|\.MP3|\.A[0-9]?|\.RAR|\.R[0-9][0-9]|\.asf|\.avi|\.iso|\.mpeg|\.mpg|\.mpga|\.ra|\.rm|\.rmvb|\.wma|\.wmp|\.wmv|\.msi)$/i;
      for (var i = 0; i < document.links.length; i++) {
        if (document.links[i].getAttribute('thunderhref')) {
          var thunderhref = document.links[i].getAttribute('thunderhref');
          document.links[i].href = thunderhref;
        }
        if (sufix.test(document.links[i].href) ||
            document.links[i].getAttribute('thunderhref')) {
          if (document.links[i].onclick) {
            document.links[i].click = document.links[i].onclick;
            document.links[i].onclick = function() {
              this.click();
              return download(this);
            }
          } else {
            document.links[i].onclick = function() {
              return download(this);
            }
          }

          if (!useExperimentalAPI) {
            document.links[i].oncontextmenu = function () {
              if (contextMenuStatus) {

                // Not show the original context menu.
                window.event.returnValue = false;
                if (!document.getElementById('dh-menu')) {
                  generateContextMenu();
                  console.log("generate");
                }
                showMyMenu(this);
              } else {
                return true;
              }
            };
          }
        }
      }
    });
}

function download(link) {
  if (event.ctrlKey) {
    return true;
  }
  if (downloader == 'thunder' || downloader == 'miniFlashget' ||
      downloader == 'flashget' || downloader == 'miniThunder') {
    var message_ = {command : '', content : ['', '', '']};
    message_.command = downloader;
    message_.content[0] = link.href;
    message_.content[1] = trim(link.innerText || link.textContent) || ' ';
    message_.content[2] = location.href;
    chrome.extension.sendRequest(message_);
    return false;
  } else {
    return true;
  }
}

function downloadByExternalDownloader(link, externalDownloader) {
  var message_ = {command : externalDownloader, content : ['', '', '']};
  message_.content[0] = link.href;
  message_.content[1] = trim(link.innerText || link.textContent) || ' ';
  message_.content[2] = location.href;
  chrome.extension.sendRequest(message_);
}

function disableContextMenu() {
  chrome.extension.sendRequest(
      {command: 'disable'}, function(response) {
          contextMenuStatus = eval(response);});
}

function showMyMenu(link) {
  var contextMenu = document.getElementById('dh-menu');
  var bodyStyle = window.getComputedStyle (document.body, "style");
  var top = event.pageY;
  var left = event.pageX;
  if (bodyStyle.position == 'relative') {
    left -= parseInt(bodyStyle.marginLeft);
    top -= parseInt(bodyStyle.marginTop);
  }
  contextMenu.style.top = top + 'px';
  contextMenu.style.left = left + 'px';
  contextMenu.style.display = 'block';
  var thunderLink = (link.href.toLocaleLowerCase().indexOf('thunder://') == 0);
  var mFlashget = document.getElementById('mFlashget');
  if (flashgetStatus && !thunderLink) {
    mFlashget.parentNode.className = 'dh-menu-item';
    mFlashget.onclick = function() {
      downloadByExternalDownloader(link, 'flashget');
    }
  } else {
    mFlashget.parentNode.className = 'dh-menu-item-disable';
  }
  var mMiniFlashget = document.getElementById('mMiniFlashget');
  if (miniFlashgetStatus && !thunderLink) {
    mMiniFlashget.parentNode.className = 'dh-menu-item';
    mMiniFlashget.onclick = function() {
      downloadByExternalDownloader(link, 'miniFlashget');
    }
  } else {
    mMiniFlashget.parentNode.className = 'dh-menu-item-disable';
  }
  
  var mThunder = document.getElementById('mThunder');
  if (thunderStatus) {
    mThunder.parentNode.className = 'dh-menu-item';
    mThunder.onclick = function() {
      downloadByExternalDownloader(link, 'thunder');
    }
  } else {
    mThunder.parentNode.className = 'dh-menu-item-disable';
  }
// nonsupport web thunder now
//  var mWebThunder = document.getElementById('mWebThunder');

//  if (webThunderStatus) {
//    mWebThunder.parentNode.className = 'dh-menu-item';
//    mWebThunder.onclick = function() {
//      downloadByExternalDownloader(link, 'webThunder');
//    }
//  } else {
//    mWebThunder.parentNode.className = 'dh-menu-item-disable';
//  }

  var mMiniThunder = document.getElementById('mMiniThunder');
  if (miniThunderStatus) {
    mMiniThunder.parentNode.className = 'dh-menu-item';
    mMiniThunder.onclick = function() {
      downloadByExternalDownloader(link, 'miniThunder');
    }
  } else {
    mMiniThunder.parentNode.className = 'dh-menu-item-disable';
  }

  var mCopyLink = document.getElementById('mCopyLink');
  mCopyLink.onclick = function() {
    chrome.extension.sendRequest({command: 'copyLink', content:link.href });
  }
  document.getElementById('mChrome').href = link.href;
  document.getElementById('mContextMenu').onclick = function() {
    disableContextMenu();
  }
}

function trim(str) {
  return str.replace(/(^\s*)|(\s*$)/g, '').replace(/\n/g, ' ').replace(/\r/g, ' ').replace(/"/g,'\\\"');
}

function initLinks(links, array) {
  for (var i = 0; i < links.length; i++) {
    var url = links[i].src||links[i].href;
    var text = trim(links[i].alt||links[i].innerText||links[i].textContent)||" ";
    array.push({url: url, text: text});
  }
}

function getAllLink() {
  var ret_ = [];

  var link_ = document.links;
  initLinks(link_, ret_);
  
  link_ = document.images;
  initLinks(link_, ret_);

  // Get frame links
  var frames = document.getElementsByTagName("frame");
  if (frames.length > 0) {
    for (var i = 0; i < frames.length; i++) {
      var frames_link = frames[i].contentDocument.links;
      var frames_img = frames[i].contentDocument.images;
      initLinks(frames_link, ret_);
      initLinks(frames_img, ret_);
    }
  }
  return ret_;
}

function downloadAll(url, downlaoder) {
  if (url == location.href) {
    if (downlaoder == 'thunder') {
      sendDownloadCommandToBg('thunder');
    } else if (downlaoder == 'flashget') {
      downloadAllByFlashget();
    }
  }
}

function checkLinks() {
  var links = getAllLink();
  var msg = 'haveLinks'
  if (links.length < 1) {
    msg = 'noLinks'
  }
  chrome.extension.sendRequest({msg: msg});
}

function sendDownloadCommandToBg(type) {
  var links_ = getAllLink();
  if (links_.length < 1) {
    //chrome.extension.sendRequest({'msg': 'noLink'});
    return;
  }
  
  var message_ = {command : type, content : links_};
  var port_ = chrome.extension.connect();
  port_.postMessage(message_);

}

function downloadAllByFlashget() {
  var links_ = getAllLink();
  if (links_.length < 1) {
    //chrome.extension.sendRequest({'msg': 'noLink'});
    return;
  }

  var message_ = {command : '', content : ''};
  var port_ = chrome.extension.connect();
  var script_ = 'pluginobj.flashgetDownloadAll("' + location.href + '",';
  for (var i = 0 ; i < links_.length ; i++) {
    script_ += '"' + (links_[i].src||links_[i].href) + '","' +
               (trim(links_[i].alt||links_[i].innerText||links_[i].textContent)||" ") + '",';
  }
  script_ += '0)';
  console.log(script_);
  message_.command = 'FlashgetEnd';
  message_.content = script_;
  port_.postMessage(message_);
}


init();
=======
var npDownload = {
  defaultDownloader: 'chrome_downloader',
  contextMenu: true,
  contextMenuList: [],
  useContextMenuAPI: false,
  getPageLink: function() {
    var ret_ = [];

    var link_ = document.links;
    var setLinksToArray = function(links, array) {
      for (var i = 0; i < links.length; i++) {
      var url = links[i].src||links[i].href;
      var text = npDownload.trim(links[i].alt || links[i].innerText ||
          links[i].textContent) || " ";
      array.push({url: url, text: text});
  }
    }
    setLinksToArray(link_, ret_);

    link_ = document.images;
    setLinksToArray(link_, ret_);

    // Get frame links
    var frames = document.getElementsByTagName("frame");
    if (frames.length > 0) {
      for (var i = 0; i < frames.length; i++) {
        var frames_link = frames[i].contentDocument.links;
        var frames_img = frames[i].contentDocument.images;
        setLinksToArray(frames_link, ret_);
        setLinksToArray(frames_img, ret_);
      }
    }
    return ret_;
  },

  trim: function(str) {
    return str.replace(/(^\s*)|(\s*$)/g, '').replace(/\n/g, ' ').replace(/\r/g, ' ').replace(/"/g,'\\\"');
  },

  overwritePageLinks: function() {
    var sufix = /(\.CRX|\.ZIP|\.EXE|\.BIN|\.GZ|\.Z|\.TAR|\.ARJ|\.LZH|\.MP3|\.A[0-9]?|\.RAR|\.R[0-9][0-9]|\.asf|\.avi|\.iso|\.mpeg|\.mpg|\.mpga|\.ra|\.rm|\.rmvb|\.wma|\.wmp|\.wmv|\.msi)$/i;
    for (var i = 0; i < document.links.length; i++) {
      if (document.links[i].getAttribute('thunderhref')) {
        var thunderhref = document.links[i].getAttribute('thunderhref');
        document.links[i].href = thunderhref;
      }
      if (sufix.test(document.links[i].href) ||
          document.links[i].getAttribute('thunderhref')) {
        if (document.links[i].onclick) {
          document.links[i].click = document.links[i].onclick;
          document.links[i].onclick = function() {
            this.click();
            return npDownload.download(this);
          }
        } else {
          document.links[i].onclick = function() {
            return npDownload.download(this);
          }
        }
        if (!npDownload.useContextMenuAPI) {
          document.links[i].oncontextmenu = function () {
            // Not show the original context menu.
            if (npDownload.contextMenu) {
              window.event.returnValue = false;
              var contextMenu = npDownload.generateContextMenu(this);
              npDownload.showMyContextMenu(contextMenu);
            }
          };
        }
      }
    }
  },

  download: function(link) {
    if (this.defaultDownloader != 'chrome_downloader') {
      npDownload.sendDownloadCommandToBg(link, this.defaultDownloader);
      return false;
    } else {
      return true;
    }
  },

  sendRequest: function(message, responseCallback) {
    chrome.extension.sendRequest(message, responseCallback);
  },

  onRequest: function() {
    chrome.extension.onRequest.addListener( function(request, sender, response) {
      if (request.msg == 'init_check') {
        npDownload.contextMenu = eval(request.contextMenu);
        npDownload.defaultDownloader = request.defaultDownloader;
        npDownload.contextMenuList = request.contextMenuList;
        npDownload.useContextMenuAPI = request.useContextMenuAPI;
        response(true);
      }
    })
  },

  generateContextMenu: function(link) {
    var otherItem = [
      {name: 'copyLink', showName: 'menu_copy_link'},
      {name: '', showName: ''},
      {name: 'contextMenu', showName: 'menu_disable'}
    ];
    var contextMenuList = npDownload.contextMenuList.concat(otherItem);
    var contextMenu = document.createElement('DIV');
    contextMenu.id = 'dh-menu';
    var privateLink = ['flashget://', 'thunder://'];
    var isPrivateLink = false;
    var index;
    for (index = 0; index < privateLink.length; index++) {
      if (link.href.toLowerCase().indexOf(privateLink[index]) == 0) {
        isPrivateLink = true;
        break;
      }
    }
    for (var i = 0; i < contextMenuList.length; i++) {
      var item = contextMenuList[i];
      var menuItem = document.createElement('DIV');

      if (item.name) {
        if (isPrivateLink && item.privateLink &&
            privateLink[index] != item.privateLink) {
          menuItem.className = 'dh-menu-item-disable';
        } else {
          menuItem.className = 'dh-menu-item';
        }
        var a = document.createElement('A');
        a.href = 'javascript:void(0)';
        a.innerText = chrome.i18n.getMessage(item.showName);
        menuItem.appendChild(a);
        contextMenu.appendChild(menuItem);
        if (item.name == 'chrome_downloader') {
          a.href = link.href;
        } else {
          (function(message, element) {
            element.onclick = function() {
              if (message == 'contextMenu') {
                npDownload.contextMenu = false;
              }
              npDownload.sendDownloadCommandToBg(link, message);
            }
          })(item.name, a);
        }
      } else {
        menuItem.className = 'dh-menu-line';
      }
      contextMenu.appendChild(menuItem);
    }
    return contextMenu;
  },

  checkFrameByUrlAndDown: function(url, downloader) {
    if (url == location.href) {
      this.sendDownloadAllCommandToBg(downloader);
    }
  },

  sendDownloadCommandToBg: function(link, message) {
    var message_ = {msg: message, link: {}, pageUrl: location.href};
    message_.link.url = link.href;
    message_.link.text = this.trim(link.innerText || link.textContent) || ' ';
    message_.link.pageUrl = location.href;
    this.sendRequest(message_, function() {});
  },

  sendDownloadAllCommandToBg: function(downloader) {
    var links = this.getPageLink();
    var message_ = {msg: 'downloadAll', downloader: downloader,
        links: links, pageUrl: location.href};
    this.sendRequest(message_, function() {});
  },

  showMyContextMenu: function(contextMenu) {
    if (contextMenu.id && document.getElementById(contextMenu.id)) {
      document.getElementById(contextMenu.id).parentNode.removeChild(
          document.getElementById(contextMenu.id))
    }
    var bodyStyle = window.getComputedStyle(document.body, "style");
    var top = event.pageY;
    var left = event.pageX;
    if (bodyStyle.position == 'relative') {
      left -= parseInt(bodyStyle.marginLeft);
      top -= parseInt(bodyStyle.marginTop);
    }
    contextMenu.style.top = top + 'px';
    contextMenu.style.left = left + 'px';
    contextMenu.style.display = 'block';

    document.body.appendChild(contextMenu);
    document.onclick = function() {
      if (document.getElementById(contextMenu.id)) {
        contextMenu.parentNode.removeChild(contextMenu);
        console.log("remove");
      }
    }
  },

  loadContextMenuCss: function() {
    var cssRef = document.createElement('link');
    cssRef.id = 'drag-style';
    cssRef.rel = 'stylesheet';
    cssRef.href = chrome.extension.getURL('style.css');
    cssRef.type = 'text/css';
    document.getElementsByTagName('head')[0].appendChild(cssRef);
  },

  init: function() {
    this.loadContextMenuCss();
    this.onRequest();
    this.sendRequest({msg: 'init_loaded'}, function(response) {
      if (response) {
        npDownload.contextMenu = eval(response.contextMenu);
        npDownload.defaultDownloader = response.defaultDownloader;
        npDownload.useContextMenuAPI = eval(response.useContextMenuAPI);
        npDownload.contextMenuList = response.contextMenuList;
        npDownload.overwritePageLinks();
      }
    });
  }
}
npDownload.init();
>>>>>>> refactor
