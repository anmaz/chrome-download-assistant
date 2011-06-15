var npDownload = {
  defaultDownloader: 'chrome_downloader',
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
        npDownload.defaultDownloader = request.defaultDownloader;
        response(true);
      }
    })
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

  init: function() {
    this.onRequest();
    this.sendRequest({msg: 'init_loaded'}, function(response) {
      if (response) {
        npDownload.defaultDownloader = response.defaultDownloader;
        npDownload.overwritePageLinks();
      }
    });
  }
}
npDownload.init();
