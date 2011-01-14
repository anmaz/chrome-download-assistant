var plugin = document.getElementById('pluginId');
var useContextMenuAPI = true;
localStorage['defaultDownloader'] = localStorage['defaultDownloader'] || 'chrome_downloader';
localStorage['contextMenu'] = localStorage['contextMenu'] || 'true';

chrome.extension.onRequest.addListener(function(request, sender, response) {
  switch(request.msg) {
    case 'init_loaded':
      response({
        'defaultDownloader': localStorage['defaultDownloader'],
        'contextMenu': localStorage['contextMenu'],
        'useContextMenuAPI': useContextMenuAPI,
        'contextMenuList': downloaderManager.getEnableDownloader(plugin)});
      break;
    case 'thunder':
    case 'mini_thunder':
    case 'flashget':
    case 'mini_flashget':
    case 'qq_whirlwind':
    case 'emule':
    case 'orbit':
    case 'idm':
    case 'fdm':
      downloaderManager.downloader(request.msg, request.link, plugin, request.pageUrl).download();
      break;
    case 'copyLink':
      downloaderManager.copyLinkToClipboard(plugin, request.link.url);
      break;
    case 'contextMenu':
      localStorage['contextMenu'] = 'false';
      response({contextMenu: false});
      break;
    case 'downloadAll':
      downloaderManager.downloader(request.downloader, request.links, plugin, request.pageUrl).downloadAll();
      break;
  }
});

 chrome.tabs.onSelectionChanged.addListener(function(tabId) {
   var js = 'chrome.tabs.executeScript(' + tabId + ', {file: "npdownload.js"});';
    chrome.tabs.sendRequest(tabId, {
      'msg': 'init_check',
      'defaultDownloader': localStorage['defaultDownloader'],
      'contextMenu': localStorage['contextMenu'],
      'useContextMenuAPI': useContextMenuAPI,
      'contextMenuList': downloaderManager.getEnableDownloader(plugin)
    }, function(response) {
      // If content script is loaded
      if (response) {
        js = '';
      }
    });
    setTimeout(function() {
      eval(js);
    }, 500);
 });

if (useContextMenuAPI) {
 createContextMenu(plugin);
}

function createContextMenu(plugin) {
  var enableDownloaders = downloaderManager.getEnableDownloader(plugin);
  for (var i = 0; i < enableDownloaders.length; i++) {
    var downloader = enableDownloaders[i];
    if (!downloader.isSystem) {
      var title = chrome.i18n.getMessage(downloader.showName);
      var name = downloader.name;
      contextMenuDownload(title, name, plugin);
      if (downloader.supportDownloadAll) {
        title = chrome.i18n.getMessage(downloader.showName2);
        contextMenuDownloadAll(title, name);
      }
    }
  }
}

function isWindowsPlatform() {
  return navigator.userAgent.toLowerCase().indexOf('windows') > -1;
}

if (!isWindowsPlatform()) {
  createLinuxContextMenu();
}

function createLinuxContextMenu() {
  var rowCounts = localStorage['rowCounts'];
  for (var i = 0; i < rowCounts; ++i) {
    var downloaderConfigureArr  = localStorage['downloaderConfigure' + i].split(',');
    contextMenuDownload(downloaderConfigureArr[1], downloaderConfigureArr, plugin)
  }
}

function contextMenuDownload(title, downloader, plugin) {
  chrome.contextMenus.create({title: title, contexts: ['link'],
      onclick: function(info, tab) {
    var link = {};
    link.url = info.linkUrl;
    link.text = info.selectionText || ' ';
    link.pageUrl = info.pageUrl ;
    if (isWindowsPlatform()) {
      downloaderManager.downloader(downloader, link, plugin, info.pageUrl).download();
    } else {
      downloaderManager.linuxDownload(downloader, link, plugin, info.pageUrl);
    }
  }}, function() {});
}

function contextMenuDownloadAll(title, downloader) {
  chrome.contextMenus.create({title: title, contexts: ['all'],
      onclick: function(info, tab) {
    if (info.frameUrl) {
      chrome.tabs.executeScript(null,
          {code: 'npDownload.checkFrameByUrlAndDown("' +
          info.frameUrl + '", "' + downloader + '")', allFrames: true})
    } else {
      chrome.tabs.executeScript(null,
          {code: 'npDownload.sendDownloadAllCommandToBg("' +
          downloader + '");'})
    }
  }});
}

