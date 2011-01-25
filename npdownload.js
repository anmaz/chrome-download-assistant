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
    return str.replace(/(^\s*)|(\s*$)/g, '').replace(/\n/g, ' ').
        replace(/\r/g, ' ').replace(/"/g,'\\\"');
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
    chrome.extension.onRequest.addListener(function(request, sender, response) {
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