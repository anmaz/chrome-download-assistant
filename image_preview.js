var imagePreview = {
  imagesList: [],
  imagesPosition: [],
  markedImages: [],
  currentImages: [],
  downloader: '',
  sort: '',
  startX: 0,
  startY: 0,
  endX: 0,
  endY: 0,
  bodyOverflowStyle: '',
  isMouseDown: false,
  createPreviewWindow: function() {
    imagePreview.sort = 'desc';
    var previewBox = document.createElement('DIV');
    previewBox.style.position = 'fixed';
    previewBox.style.top = 0;
    previewBox.style.left = 0;
    this.setCloseBtn(previewBox);
    var previewMask = document.createElement('DIV');
    previewMask.style.width = window.innerWidth + 'px';
    previewMask.style.height = window.innerHeight + 'px';
    previewMask.style.opacity = 0.8;
    previewMask.style.background = '#000000';
    previewMask.className = 'dh-preview-window';
    previewBox.appendChild(previewMask);
    var previewWindow = document.createElement('DIV');
    previewWindow.style.width = window.innerWidth + 'px';
    previewWindow.style.height = window.innerHeight + 'px';
    previewWindow.className = 'dh-preview-window';
    previewBox.appendChild(previewWindow);
    document.body.appendChild(previewBox);
    return previewWindow;
  },

  getAllImage: function() {
    var images = document.images;
    return images;
  },

  initImages: function() {
    var images = this.getAllImage();
    this.imagesList = [];
    for (var i = 0; i < images.length; i++) {
      var image = images[i];
      var size = this.getOriginalImageSize(image);
      var suffix = this.getSuffix(image.src);
      var area = size.width * size.height;
      var text = npDownload.trim(image.alt || image.innerText ||
          image.textContent) || " ";
      if (size.width > 5 && size.height > 5) {
        this.imagesList.push({url: image.src, text: text, width: size.width,
          height: size.height, area: area, suffix: suffix});
      }
    }
  },

  getSuffix: function(fileName) {
    var suffix = '';
    var index = fileName.lastIndexOf('.');
    suffix = fileName.substring(index+1, fileName.length);
    return suffix;
  },

  getOriginalImageSize: function(image) {
    var width = image.naturalWidth;
    var height = image.naturalHeight;
    return {width: width, height: height};
  },


  showImage: function(imagesList, imageFrame) {
    //imagePreview.currentImages = imagesList;
    if (imagePreview.sort == 'asc') {
      imagePreview.currentImages = imagePreview.sortByAsc(imagesList);
    } else if (imagePreview.sort == 'desc') {
      imagePreview.currentImages = imagePreview.sortByDesc(imagesList);
    }
    var ul = document.createElement('UL');
    imageFrame.appendChild(ul);
    for (var i = 0; i < imagesList.length; i++) {
      var image = imagesList[i];
      var li = document.createElement('LI');
      var img = document.createElement('IMG');
      img.src = image.url;
      if (image.height > 120) {
        img.height = 120;
      }
      (function(image){
        image.addEventListener('click', function() {
          if (event.ctrlKey) {
            if (this.name && this.name == 'dh-img-marked') {
              imagePreview.unmark(this);
            } else {
              imagePreview.mark(this);
            }
          } else {
            imagePreview.unmarkAll();
            imagePreview.mark(this);
          }
          event.stopPropagation();
        }, false);
      })(img);
      li.appendChild(img);
      var size = document.createElement('div');
      size.innerText = image.width + ' × ' + image.height;
      li.appendChild(size);
      ul.appendChild(li);
      this.imagesPosition.push({image: img, startX: img.offsetLeft,
          startY: img.offsetTop, endX: img.offsetLeft + img.clientWidth,
          endY: img.offsetTop + img.clientHeight});
    }
  },

  createNavigationBar: function() {
    var navBar = document.createElement('div');
    navBar.className = 'dh-nav-bar';
    return navBar;
  },

  createImageFrame: function() {
    var imageFrame = document.createElement('DIV');
    imageFrame.style.width = window.innerWidth - 200 + 'px';
    imageFrame.className = 'dh-image-frame';
    imageFrame.id = 'dh-image-frame';
    return imageFrame;
  },

  show: function() {
    var bodyStyle = window.getComputedStyle(document.body, "style");
    this.bodyOverflowStyle = bodyStyle.overflow;
    document.body.style.overflow = 'hidden';
    this.initImages();
    var previewWindow = this.createPreviewWindow();
    var imageFrame = this.createImageFrame();
    var navBar = this.createNavigationBar();
    this.createNavMenu(navBar);
    this.createDownloaderMenu(navBar);
    previewWindow.appendChild(navBar);
    previewWindow.appendChild(imageFrame);
    this.showImage(this.imagesList, imageFrame);
    imageFrame.addEventListener('mousedown', function() {
      imagePreview.onMouseDown(this);
    }, false);
    document.addEventListener('mousemove', function() {
      imagePreview.onMouseMove(imageFrame);
    }, false);
    document.addEventListener('mouseup', this.onMouseUp, false);
  },

  onMouseDown: function(element) {
    imagePreview.isMouseDown = true;
    imagePreview.startX = event.clientX + element.scrollLeft;
    imagePreview.startY = event.clientY + element.scrollTop;
    var div = document.createElement('DIV');
    div.id = 'dh-select-frame';
    element.appendChild(div);
    event.preventDefault();
  },

  onMouseMove: function(element) {

    if (imagePreview.isMouseDown) {
      var xPosition = event.clientX + element.scrollLeft;
      var yPosition = event.clientY + element.scrollTop;
      var top = imagePreview.startY;
      var left = imagePreview.startX;
      var width = Math.abs(xPosition - imagePreview.startX);
      var height = Math.abs(yPosition - imagePreview.startY);
      var selectFrame = document.getElementById('dh-select-frame');
      if (imagePreview.startX > xPosition) {
        left = xPosition;
      }
      if (imagePreview.startY > yPosition) {
        top = yPosition;
      }
      if (window.innerHeight + element.scrollTop < yPosition + 20) {
        element.scrollTop  = yPosition - window.innerHeight + 20;
      }
      if (yPosition -20 < element.scrollTop) {
        element.scrollTop -= 20;
      }
      top -= element.scrollTop;
      selectFrame.style.top = top + 'px';
      selectFrame.style.left = left + 'px';
      selectFrame.style.width = width + 'px';
      selectFrame.style.height = height + 'px';
      var startX = imagePreview.startX < xPosition ? imagePreview.startX : xPosition;
      var startY = imagePreview.startY < yPosition ? imagePreview.startY : yPosition;
      var selectBox = {startX: startX, startY: startY, endX: startX + width,
          endY: startY + height};
      imagePreview.checkImageIntersection(selectBox);
    }
  },

  onMouseUp: function() {
    imagePreview.isMouseDown = false;
    var selectFrame = document.getElementById('dh-select-frame');
    if (selectFrame) {
      selectFrame.parentNode.removeChild(selectFrame);
    }
  },

  mark: function(element) {
    element.className = 'dh-selected';
    element.name = 'dh-img-marked';
  },

  unmark: function(element) {
    element.className = '';
    element.name = '';
  },

  unmarkAll: function() {
    var markedElements = document.getElementsByName('dh-img-marked');
    for (var i = 0; i < markedElements.length; i = 0) {
      var node = markedElements[0];
      imagePreview.unmark(node);
    }
  },

  isIntersect: function(element1, element2) {
    var isIntersect = true;
    var newStartX = Math.max(element1.startX, element2.startX);
    var newStartY = Math.max(element1.startY, element2.startY);
    var newEndX = Math.min(element1.endX, element2.endX);
    var newEndY = Math.min(element1.endY, element2.endY);

    if (newStartX > newEndX || newStartY > newEndY) {
      isIntersect = false;
    }
    return isIntersect;
  },

  checkImageIntersection: function(selectBox) {
    for (var i = 0; i < this.imagesPosition.length; i++) {
      var node = this.imagesPosition[i];
      if (this.isIntersect(node, selectBox)) {
        this.mark(node.image);
      } else {
        this.unmark(node.image);
      }
    }
  },

  createNavMenu: function(navBar) {
    var ul = document.createElement('UL');
    var menuList = [
      {menuName: 'menu_sort_size_desc', func: 'imagePreview.sortImageDesc()', marked: true, group: 'dh-image-sort'},
      {menuName: 'menu_sort_size_asc', func: 'imagePreview.sortImageAsc()', group: 'dh-image-sort'},
      {menuName: 'menu_filter'},
      {menuName: 'menu_screening_any', func: 'imagePreview.filterImageBySuffix("any")', marked: true, group: 'dh-image-format'},
      {menuName: 'menu_screening_jpg', func: 'imagePreview.filterImageBySuffix("jpg")', group: 'dh-image-format'},
      {menuName: 'menu_screening_png', func: 'imagePreview.filterImageBySuffix("png")', group: 'dh-image-format'},
      {menuName: 'menu_screening_gif', func: 'imagePreview.filterImageBySuffix("gif")', group: 'dh-image-format'},
      {menuName: 'menu_screening_other', func: 'imagePreview.filterImageBySuffix("other")', group: 'dh-image-format'}
    ];
    for (var i = 0; i < menuList.length; i++) {
      var menu = menuList[i];
      var li = document.createElement('LI');
      if (menu.func) {
        var a = document.createElement('A');
        a.innerText = chrome.i18n.getMessage(menu.menuName);
        a.name = menu.group;
        if (menu.marked) {
          a.className = 'marked';
        }
        (function(menu, func, group) {
          menu.addEventListener('click', function() {
            eval(func);
            imagePreview.markSelectMenu(menu, group);
          }, false);
        })(a, menu.func, menu.group);
        li.appendChild(a);
      } else {
        li.className = 'space';
      }
      ul.appendChild(li);
    }
    navBar.appendChild(ul);
  },

  sortByAsc: function(array) {
    for(var i=0; i < array.length-1;i++) {
      var flag = false;
      for(var j = array.length-1; j > i; j--) {
        if(array[j].area < array[j-1].area) {
          var tmp = array[j];
          array[j] = array[j-1];
          array[j-1] = tmp;
          flag = true;
        }
      }
      if(!flag) break;
    }
    return array;
  },

  sortByDesc: function(array) {
    for(var i = 0; i < array.length-1; i++) {
      var flag = false;
      for(var j = array.length-1; j > i; j--) {
        if(array[j].area > array[j-1].area) {
          var tmp = array[j];
          array[j] = array[j-1];
          array[j-1] = tmp;
          flag = true;
        }
      }
      if(!flag) break;
    }
    return array;
  },

  sortImageAsc: function() {
    imagePreview.sort = 'asc';
    var images = imagePreview.sortByAsc(imagePreview.currentImages);
    var imageFrame = document.getElementById('dh-image-frame');
    imageFrame.innerHTML = '';
    imagePreview.showImage(images, imageFrame);
  },

  sortImageDesc: function() {
    imagePreview.sort = 'desc';
    var images = imagePreview.sortByDesc(imagePreview.currentImages);
    var imageFrame = document.getElementById('dh-image-frame');
    imageFrame.innerHTML = '';
    imagePreview.showImage(images, imageFrame);
  },

  filterImageBySuffix: function(suffix) {
    var newImageList = [];
    var suffixMap = ['any', 'jpg', 'png', 'gif'];
    var index = suffixMap.indexOf(suffix);
    if (index == 0) {
      newImageList = imagePreview.imagesList;
    } else {
      for (var i = 0; i < imagePreview.imagesList.length; i++) {
        var image = imagePreview.imagesList[i];
        var imageSuffix = image.suffix.toString().toLowerCase();
        if (suffixMap.indexOf(imageSuffix) == index) {
          newImageList.push(image);
        } else if (suffixMap.indexOf(imageSuffix) < 0 && index < 0){
          newImageList.push(image);
        }
      }
    }

    var imageFrame = document.getElementById('dh-image-frame');
    imageFrame.innerHTML = '';
    imagePreview.showImage(newImageList, imageFrame);
  },

  createDownloaderMenu: function(navBar) {
    var allEnableDownlaoder = npDownload.contextMenuList;
    var ul = document.createElement('UL');
    for (var i = 0; i < allEnableDownlaoder.length; i++) {
      var downloader = allEnableDownlaoder[i];
      if (downloader.supportDownloadAll) {
        var li = document.createElement('LI');
        var a = document.createElement('A');
        a.innerText = chrome.i18n.getMessage(downloader.name);
        a.name = 'dh-downloader';
        (function(element, name){
          element.onclick = function() {
            imagePreview.downloader = name;
            imagePreview.markSelectMenu(element, 'dh-downloader');
          }
        })(a, downloader.name);
        if (i == 0) {
          a.className = 'marked';
          imagePreview.downloader = downloader.name;
        }
        li.appendChild(a);
        ul.appendChild(li);
      }
    }
    navBar.appendChild(ul);
    var btnDiv = document.createElement('DIV');
    var btnDownloadSelect = document.createElement('BUTTON');
    btnDownloadSelect.innerText = chrome.i18n.getMessage('btn_download_select');
    btnDownloadSelect.addEventListener('click', function() {
      imagePreview.downloadImages('select');
    }, false);
    var btnDownloadAll = document.createElement('BUTTON');
    btnDownloadAll.innerText = chrome.i18n.getMessage('btn_download_all');
    btnDownloadAll.addEventListener('click', function() {
      imagePreview.downloadImages('all');
    }, false);
    btnDiv.appendChild(btnDownloadSelect);
    btnDiv.appendChild(btnDownloadAll);
    navBar.appendChild(btnDiv);
  },

  getMarkedImages: function() {
    var markedElements = document.getElementsByName('dh-img-marked');
    var images = [];
    for (var i = 0; i < markedElements.length; i++) {
      var url = markedElements[i].src;
      var text = npDownload.trim(markedElements[i].alt ||
          markedElements[i].innerText || markedElements[i].textContent) || " ";
      images.push({url: url, text: text});
    }
    return images;
  },

  downloadImages: function(type) {
    var links;
    var downloader = imagePreview.downloader;
    if (type == 'all') {
      links = imagePreview.currentImages;
    } else if (type == 'select') {
      links = imagePreview.getMarkedImages()
    }
    var message_ = {msg: 'downloadAll', downloader: downloader,
        links: links, pageUrl: location.href};
    npDownload.sendRequest(message_, function() {});
  },

  setCloseBtn: function(previewBox) {
    var btnClose = document.createElement('IMG');
    btnClose.src = chrome.extension.getURL('images/icon_close.png');
    btnClose.className = 'dh-btn-close';
    btnClose.onclick = function() {
      previewBox.parentNode.removeChild(previewBox);
      document.body.style.overflow = imagePreview.bodyOverflowStyle;
    }
    previewBox.appendChild(btnClose);
  },

  markSelectMenu: function(element, group) {
    var nodes = document.getElementsByName(group);
    for (var i = 0; i < nodes.length; i++) {
      if (nodes[i] == element) {
        nodes[i].className = 'marked';
      } else {
        nodes[i].className = '';
      }
    }
  }
}
