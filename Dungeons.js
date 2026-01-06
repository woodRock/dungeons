// include: shell.js
// The Module object: Our interface to the outside world. We import
// and export values on it. There are various ways Module can be used:
// 1. Not defined. We create it here
// 2. A function parameter, function(Module) { ..generated code.. }
// 3. pre-run appended it, var Module = {}; ..generated code..
// 4. External script tag defines var Module.
// We need to check if Module already exists (e.g. case 3 above).
// Substitution will be replaced with actual code on later stage of the build,
// this way Closure Compiler will not mangle it (e.g. case 4. above).
// Note that if you want to run closure, and also to use Module
// after the generated code, you will need to define   var Module = {};
// before the code. Then that object will be used in the code, and you
// can continue to use Module afterwards as well.
var Module = typeof Module != 'undefined' ? Module : {};

// --pre-jses are emitted after the Module integration code, so that they can
// refer to Module (if they choose; they can also define Module)

  if (!Module.expectedDataFileDownloads) {
    Module.expectedDataFileDownloads = 0;
  }

  Module.expectedDataFileDownloads++;
  (function() {
    // Do not attempt to redownload the virtual filesystem data when in a pthread or a Wasm Worker context.
    if (Module['ENVIRONMENT_IS_PTHREAD'] || Module['$ww']) return;
    var loadPackage = function(metadata) {

      var PACKAGE_PATH = '';
      if (typeof window === 'object') {
        PACKAGE_PATH = window['encodeURIComponent'](window.location.pathname.toString().substring(0, window.location.pathname.toString().lastIndexOf('/')) + '/');
      } else if (typeof process === 'undefined' && typeof location !== 'undefined') {
        // web worker
        PACKAGE_PATH = encodeURIComponent(location.pathname.toString().substring(0, location.pathname.toString().lastIndexOf('/')) + '/');
      }
      var PACKAGE_NAME = 'Dungeons.data';
      var REMOTE_PACKAGE_BASE = 'Dungeons.data';
      if (typeof Module['locateFilePackage'] === 'function' && !Module['locateFile']) {
        Module['locateFile'] = Module['locateFilePackage'];
        err('warning: you defined Module.locateFilePackage, that has been renamed to Module.locateFile (using your locateFilePackage for now)');
      }
      var REMOTE_PACKAGE_NAME = Module['locateFile'] ? Module['locateFile'](REMOTE_PACKAGE_BASE, '') : REMOTE_PACKAGE_BASE;
var REMOTE_PACKAGE_SIZE = metadata['remote_package_size'];

      function fetchRemotePackage(packageName, packageSize, callback, errback) {
        if (typeof process === 'object' && typeof process.versions === 'object' && typeof process.versions.node === 'string') {
          require('fs').readFile(packageName, function(err, contents) {
            if (err) {
              errback(err);
            } else {
              callback(contents.buffer);
            }
          });
          return;
        }
        var xhr = new XMLHttpRequest();
        xhr.open('GET', packageName, true);
        xhr.responseType = 'arraybuffer';
        xhr.onprogress = function(event) {
          var url = packageName;
          var size = packageSize;
          if (event.total) size = event.total;
          if (event.loaded) {
            if (!xhr.addedTotal) {
              xhr.addedTotal = true;
              if (!Module.dataFileDownloads) Module.dataFileDownloads = {};
              Module.dataFileDownloads[url] = {
                loaded: event.loaded,
                total: size
              };
            } else {
              Module.dataFileDownloads[url].loaded = event.loaded;
            }
            var total = 0;
            var loaded = 0;
            var num = 0;
            for (var download in Module.dataFileDownloads) {
            var data = Module.dataFileDownloads[download];
              total += data.total;
              loaded += data.loaded;
              num++;
            }
            total = Math.ceil(total * Module.expectedDataFileDownloads/num);
            if (Module['setStatus']) Module['setStatus'](`Downloading data... (${loaded}/${total})`);
          } else if (!Module.dataFileDownloads) {
            if (Module['setStatus']) Module['setStatus']('Downloading data...');
          }
        };
        xhr.onerror = function(event) {
          throw new Error("NetworkError for: " + packageName);
        }
        xhr.onload = function(event) {
          if (xhr.status == 200 || xhr.status == 304 || xhr.status == 206 || (xhr.status == 0 && xhr.response)) { // file URLs can return 0
            var packageData = xhr.response;
            callback(packageData);
          } else {
            throw new Error(xhr.statusText + " : " + xhr.responseURL);
          }
        };
        xhr.send(null);
      };

      function handleError(error) {
        console.error('package error:', error);
      };

      var fetchedCallback = null;
      var fetched = Module['getPreloadedPackage'] ? Module['getPreloadedPackage'](REMOTE_PACKAGE_NAME, REMOTE_PACKAGE_SIZE) : null;

      if (!fetched) fetchRemotePackage(REMOTE_PACKAGE_NAME, REMOTE_PACKAGE_SIZE, function(data) {
        if (fetchedCallback) {
          fetchedCallback(data);
          fetchedCallback = null;
        } else {
          fetched = data;
        }
      }, handleError);

    function runWithFS() {

      function assert(check, msg) {
        if (!check) throw msg + new Error().stack;
      }
Module['FS_createPath']("/", "assets", true, true);
Module['FS_createPath']("/assets", "adventurers", true, true);
Module['FS_createPath']("/assets/adventurers", "Animations", true, true);
Module['FS_createPath']("/assets/adventurers/Animations", "fbx", true, true);
Module['FS_createPath']("/assets/adventurers/Animations/fbx", "Rig_Medium", true, true);
Module['FS_createPath']("/assets/adventurers/Animations", "gltf", true, true);
Module['FS_createPath']("/assets/adventurers/Animations/gltf", "Rig_Medium", true, true);
Module['FS_createPath']("/assets/adventurers", "Assets", true, true);
Module['FS_createPath']("/assets/adventurers/Assets", "fbx(unity)", true, true);
Module['FS_createPath']("/assets/adventurers/Assets", "fbx", true, true);
Module['FS_createPath']("/assets/adventurers/Assets", "gltf", true, true);
Module['FS_createPath']("/assets/adventurers/Assets", "obj", true, true);
Module['FS_createPath']("/assets/adventurers", "Characters", true, true);
Module['FS_createPath']("/assets/adventurers/Characters", "fbx", true, true);
Module['FS_createPath']("/assets/adventurers/Characters", "gltf", true, true);
Module['FS_createPath']("/assets/adventurers", "Samples", true, true);
Module['FS_createPath']("/assets/adventurers", "Textures", true, true);
Module['FS_createPath']("/assets", "dungeons", true, true);
Module['FS_createPath']("/assets/dungeons", "Assets", true, true);
Module['FS_createPath']("/assets/dungeons/Assets", "fbx(unity)", true, true);
Module['FS_createPath']("/assets/dungeons/Assets", "fbx", true, true);
Module['FS_createPath']("/assets/dungeons/Assets", "gltf", true, true);
Module['FS_createPath']("/assets/dungeons/Assets", "obj", true, true);
Module['FS_createPath']("/assets/dungeons/Assets", "textures", true, true);
Module['FS_createPath']("/assets/dungeons", "Samples", true, true);
Module['FS_createPath']("/assets", "skeletons", true, true);
Module['FS_createPath']("/assets/skeletons", "Animations", true, true);
Module['FS_createPath']("/assets/skeletons/Animations", "fbx", true, true);
Module['FS_createPath']("/assets/skeletons/Animations/fbx", "Rig_Medium", true, true);
Module['FS_createPath']("/assets/skeletons/Animations", "gltf", true, true);
Module['FS_createPath']("/assets/skeletons/Animations/gltf", "Rig_Medium", true, true);
Module['FS_createPath']("/assets/skeletons", "assets", true, true);
Module['FS_createPath']("/assets/skeletons/assets", "fbx(unity)", true, true);
Module['FS_createPath']("/assets/skeletons/assets", "fbx", true, true);
Module['FS_createPath']("/assets/skeletons/assets", "gltf", true, true);
Module['FS_createPath']("/assets/skeletons/assets", "obj", true, true);
Module['FS_createPath']("/assets/skeletons", "characters", true, true);
Module['FS_createPath']("/assets/skeletons/characters", "fbx", true, true);
Module['FS_createPath']("/assets/skeletons/characters", "gltf", true, true);
Module['FS_createPath']("/assets/skeletons", "samples", true, true);
Module['FS_createPath']("/assets/skeletons", "texture", true, true);

      /** @constructor */
      function DataRequest(start, end, audio) {
        this.start = start;
        this.end = end;
        this.audio = audio;
      }
      DataRequest.prototype = {
        requests: {},
        open: function(mode, name) {
          this.name = name;
          this.requests[name] = this;
          Module['addRunDependency'](`fp ${this.name}`);
        },
        send: function() {},
        onload: function() {
          var byteArray = this.byteArray.subarray(this.start, this.end);
          this.finish(byteArray);
        },
        finish: function(byteArray) {
          var that = this;
          // canOwn this data in the filesystem, it is a slide into the heap that will never change
          Module['FS_createDataFile'](this.name, null, byteArray, true, true, true);
          Module['removeRunDependency'](`fp ${that.name}`);
          this.requests[this.name] = null;
        }
      };

      var files = metadata['files'];
      for (var i = 0; i < files.length; ++i) {
        new DataRequest(files[i]['start'], files[i]['end'], files[i]['audio'] || 0).open('GET', files[i]['filename']);
      }

      function processPackageData(arrayBuffer) {
        assert(arrayBuffer, 'Loading data file failed.');
        assert(arrayBuffer.constructor.name === ArrayBuffer.name, 'bad input to processPackageData');
        var byteArray = new Uint8Array(arrayBuffer);
        var curr;
        // Reuse the bytearray from the XHR as the source for file reads.
          DataRequest.prototype.byteArray = byteArray;
          var files = metadata['files'];
          for (var i = 0; i < files.length; ++i) {
            DataRequest.prototype.requests[files[i].filename].onload();
          }          Module['removeRunDependency']('datafile_Dungeons.data');

      };
      Module['addRunDependency']('datafile_Dungeons.data');

      if (!Module.preloadResults) Module.preloadResults = {};

      Module.preloadResults[PACKAGE_NAME] = {fromCache: false};
      if (fetched) {
        processPackageData(fetched);
        fetched = null;
      } else {
        fetchedCallback = processPackageData;
      }

    }
    if (Module['calledRun']) {
      runWithFS();
    } else {
      if (!Module['preRun']) Module['preRun'] = [];
      Module["preRun"].push(runWithFS); // FS is not initialized yet, wait for it
    }

    }
    loadPackage({"files": [{"filename": "/assets/adventurers/Animations/Click here for more Free Animations.url", "start": 0, "end": 155}, {"filename": "/assets/adventurers/Animations/fbx/Rig_Medium/Rig_Medium_General.fbx", "start": 155, "end": 2382855}, {"filename": "/assets/adventurers/Animations/fbx/Rig_Medium/Rig_Medium_MovementBasic.fbx", "start": 2382855, "end": 4203923}, {"filename": "/assets/adventurers/Animations/gltf/Rig_Medium/Rig_Medium_General.glb", "start": 4203923, "end": 5032163}, {"filename": "/assets/adventurers/Animations/gltf/Rig_Medium/Rig_Medium_MovementBasic.glb", "start": 5032163, "end": 5721787}, {"filename": "/assets/adventurers/Assets/fbx(unity)/arrow_bow.fbx", "start": 5721787, "end": 5742087}, {"filename": "/assets/adventurers/Assets/fbx(unity)/arrow_bow_bundle.fbx", "start": 5742087, "end": 5770579}, {"filename": "/assets/adventurers/Assets/fbx(unity)/arrow_crossbow.fbx", "start": 5770579, "end": 5790847}, {"filename": "/assets/adventurers/Assets/fbx(unity)/arrow_crossbow_bundle.fbx", "start": 5790847, "end": 5819323}, {"filename": "/assets/adventurers/Assets/fbx(unity)/axe_1handed.fbx", "start": 5819323, "end": 5847223}, {"filename": "/assets/adventurers/Assets/fbx(unity)/axe_2handed.fbx", "start": 5847223, "end": 5884643}, {"filename": "/assets/adventurers/Assets/fbx(unity)/barbarian_texture.png", "start": 5884643, "end": 5899067}, {"filename": "/assets/adventurers/Assets/fbx(unity)/bow.fbx", "start": 5899067, "end": 5930375}, {"filename": "/assets/adventurers/Assets/fbx(unity)/bow_withString.fbx", "start": 5930375, "end": 5974147}, {"filename": "/assets/adventurers/Assets/fbx(unity)/crossbow_1handed.fbx", "start": 5974147, "end": 6012751}, {"filename": "/assets/adventurers/Assets/fbx(unity)/crossbow_2handed.fbx", "start": 6012751, "end": 6059579}, {"filename": "/assets/adventurers/Assets/fbx(unity)/dagger.fbx", "start": 6059579, "end": 6084279}, {"filename": "/assets/adventurers/Assets/fbx(unity)/knight_texture.png", "start": 6084279, "end": 6097193}, {"filename": "/assets/adventurers/Assets/fbx(unity)/mage_texture.png", "start": 6097193, "end": 6111933}, {"filename": "/assets/adventurers/Assets/fbx(unity)/mug_empty.fbx", "start": 6111933, "end": 6141049}, {"filename": "/assets/adventurers/Assets/fbx(unity)/mug_full.fbx", "start": 6141049, "end": 6171525}, {"filename": "/assets/adventurers/Assets/fbx(unity)/quiver.fbx", "start": 6171525, "end": 6199377}, {"filename": "/assets/adventurers/Assets/fbx(unity)/ranger_texture.png", "start": 6199377, "end": 6212625}, {"filename": "/assets/adventurers/Assets/fbx(unity)/rogue_texture.png", "start": 6212625, "end": 6227651}, {"filename": "/assets/adventurers/Assets/fbx(unity)/shield_badge.fbx", "start": 6227651, "end": 6255007}, {"filename": "/assets/adventurers/Assets/fbx(unity)/shield_badge_color.fbx", "start": 6255007, "end": 6282331}, {"filename": "/assets/adventurers/Assets/fbx(unity)/shield_round.fbx", "start": 6282331, "end": 6310663}, {"filename": "/assets/adventurers/Assets/fbx(unity)/shield_round_barbarian.fbx", "start": 6310663, "end": 6338963}, {"filename": "/assets/adventurers/Assets/fbx(unity)/shield_round_color.fbx", "start": 6338963, "end": 6367247}, {"filename": "/assets/adventurers/Assets/fbx(unity)/shield_spikes.fbx", "start": 6367247, "end": 6400619}, {"filename": "/assets/adventurers/Assets/fbx(unity)/shield_spikes_color.fbx", "start": 6400619, "end": 6434007}, {"filename": "/assets/adventurers/Assets/fbx(unity)/shield_square.fbx", "start": 6434007, "end": 6461699}, {"filename": "/assets/adventurers/Assets/fbx(unity)/shield_square_color.fbx", "start": 6461699, "end": 6489423}, {"filename": "/assets/adventurers/Assets/fbx(unity)/smokebomb.fbx", "start": 6489423, "end": 6516779}, {"filename": "/assets/adventurers/Assets/fbx(unity)/spellbook_closed.fbx", "start": 6516779, "end": 6543911}, {"filename": "/assets/adventurers/Assets/fbx(unity)/spellbook_open.fbx", "start": 6543911, "end": 6571251}, {"filename": "/assets/adventurers/Assets/fbx(unity)/staff.fbx", "start": 6571251, "end": 6603759}, {"filename": "/assets/adventurers/Assets/fbx(unity)/sword_1handed.fbx", "start": 6603759, "end": 6631995}, {"filename": "/assets/adventurers/Assets/fbx(unity)/sword_2handed.fbx", "start": 6631995, "end": 6663815}, {"filename": "/assets/adventurers/Assets/fbx(unity)/sword_2handed_color.fbx", "start": 6663815, "end": 6695651}, {"filename": "/assets/adventurers/Assets/fbx(unity)/wand.fbx", "start": 6695651, "end": 6718959}, {"filename": "/assets/adventurers/Assets/fbx/arrow_bow.fbx", "start": 6718959, "end": 6739355}, {"filename": "/assets/adventurers/Assets/fbx/arrow_bow_bundle.fbx", "start": 6739355, "end": 6767831}, {"filename": "/assets/adventurers/Assets/fbx/arrow_crossbow.fbx", "start": 6767831, "end": 6788211}, {"filename": "/assets/adventurers/Assets/fbx/arrow_crossbow_bundle.fbx", "start": 6788211, "end": 6816703}, {"filename": "/assets/adventurers/Assets/fbx/axe_1handed.fbx", "start": 6816703, "end": 6844571}, {"filename": "/assets/adventurers/Assets/fbx/axe_2handed.fbx", "start": 6844571, "end": 6882023}, {"filename": "/assets/adventurers/Assets/fbx/barbarian_texture.png", "start": 6882023, "end": 6896447}, {"filename": "/assets/adventurers/Assets/fbx/bow.fbx", "start": 6896447, "end": 6927627}, {"filename": "/assets/adventurers/Assets/fbx/bow_withString.fbx", "start": 6927627, "end": 6970887}, {"filename": "/assets/adventurers/Assets/fbx/crossbow_1handed.fbx", "start": 6970887, "end": 7009571}, {"filename": "/assets/adventurers/Assets/fbx/crossbow_2handed.fbx", "start": 7009571, "end": 7056351}, {"filename": "/assets/adventurers/Assets/fbx/dagger.fbx", "start": 7056351, "end": 7081099}, {"filename": "/assets/adventurers/Assets/fbx/knight_texture.png", "start": 7081099, "end": 7094013}, {"filename": "/assets/adventurers/Assets/fbx/mage_texture.png", "start": 7094013, "end": 7108753}, {"filename": "/assets/adventurers/Assets/fbx/mug_empty.fbx", "start": 7108753, "end": 7137789}, {"filename": "/assets/adventurers/Assets/fbx/mug_full.fbx", "start": 7137789, "end": 7168217}, {"filename": "/assets/adventurers/Assets/fbx/quiver.fbx", "start": 7168217, "end": 7196117}, {"filename": "/assets/adventurers/Assets/fbx/ranger_texture.png", "start": 7196117, "end": 7209365}, {"filename": "/assets/adventurers/Assets/fbx/rogue_texture.png", "start": 7209365, "end": 7224391}, {"filename": "/assets/adventurers/Assets/fbx/shield_badge.fbx", "start": 7224391, "end": 7251571}, {"filename": "/assets/adventurers/Assets/fbx/shield_badge_color.fbx", "start": 7251571, "end": 7278735}, {"filename": "/assets/adventurers/Assets/fbx/shield_round.fbx", "start": 7278735, "end": 7307019}, {"filename": "/assets/adventurers/Assets/fbx/shield_round_barbarian.fbx", "start": 7307019, "end": 7335255}, {"filename": "/assets/adventurers/Assets/fbx/shield_round_color.fbx", "start": 7335255, "end": 7363491}, {"filename": "/assets/adventurers/Assets/fbx/shield_spikes.fbx", "start": 7363491, "end": 7396639}, {"filename": "/assets/adventurers/Assets/fbx/shield_spikes_color.fbx", "start": 7396639, "end": 7429803}, {"filename": "/assets/adventurers/Assets/fbx/shield_square.fbx", "start": 7429803, "end": 7457463}, {"filename": "/assets/adventurers/Assets/fbx/shield_square_color.fbx", "start": 7457463, "end": 7485155}, {"filename": "/assets/adventurers/Assets/fbx/smokebomb.fbx", "start": 7485155, "end": 7512527}, {"filename": "/assets/adventurers/Assets/fbx/spellbook_closed.fbx", "start": 7512527, "end": 7539163}, {"filename": "/assets/adventurers/Assets/fbx/spellbook_open.fbx", "start": 7539163, "end": 7565975}, {"filename": "/assets/adventurers/Assets/fbx/staff.fbx", "start": 7565975, "end": 7598595}, {"filename": "/assets/adventurers/Assets/fbx/sword_1handed.fbx", "start": 7598595, "end": 7626815}, {"filename": "/assets/adventurers/Assets/fbx/sword_2handed.fbx", "start": 7626815, "end": 7658539}, {"filename": "/assets/adventurers/Assets/fbx/sword_2handed_color.fbx", "start": 7658539, "end": 7690295}, {"filename": "/assets/adventurers/Assets/fbx/wand.fbx", "start": 7690295, "end": 7713651}, {"filename": "/assets/adventurers/Assets/gltf/arrow_bow.bin", "start": 7713651, "end": 7716299}, {"filename": "/assets/adventurers/Assets/gltf/arrow_bow.gltf", "start": 7716299, "end": 7719354}, {"filename": "/assets/adventurers/Assets/gltf/arrow_bow_bundle.bin", "start": 7719354, "end": 7734698}, {"filename": "/assets/adventurers/Assets/gltf/arrow_bow_bundle.gltf", "start": 7734698, "end": 7737782}, {"filename": "/assets/adventurers/Assets/gltf/arrow_crossbow.bin", "start": 7737782, "end": 7740430}, {"filename": "/assets/adventurers/Assets/gltf/arrow_crossbow.gltf", "start": 7740430, "end": 7743501}, {"filename": "/assets/adventurers/Assets/gltf/arrow_crossbow_bundle.bin", "start": 7743501, "end": 7758845}, {"filename": "/assets/adventurers/Assets/gltf/arrow_crossbow_bundle.gltf", "start": 7758845, "end": 7761942}, {"filename": "/assets/adventurers/Assets/gltf/axe_1handed.bin", "start": 7761942, "end": 7774466}, {"filename": "/assets/adventurers/Assets/gltf/axe_1handed.gltf", "start": 7774466, "end": 7777546}, {"filename": "/assets/adventurers/Assets/gltf/axe_2handed.bin", "start": 7777546, "end": 7799506}, {"filename": "/assets/adventurers/Assets/gltf/axe_2handed.gltf", "start": 7799506, "end": 7802584}, {"filename": "/assets/adventurers/Assets/gltf/barbarian_texture.png", "start": 7802584, "end": 7817008}, {"filename": "/assets/adventurers/Assets/gltf/bow.bin", "start": 7817008, "end": 7835352}, {"filename": "/assets/adventurers/Assets/gltf/bow.gltf", "start": 7835352, "end": 7838400}, {"filename": "/assets/adventurers/Assets/gltf/bow_withString.bin", "start": 7838400, "end": 7887416}, {"filename": "/assets/adventurers/Assets/gltf/bow_withString.gltf", "start": 7887416, "end": 7891742}, {"filename": "/assets/adventurers/Assets/gltf/crossbow_1handed.bin", "start": 7891742, "end": 7916942}, {"filename": "/assets/adventurers/Assets/gltf/crossbow_1handed.gltf", "start": 7916942, "end": 7920025}, {"filename": "/assets/adventurers/Assets/gltf/crossbow_2handed.bin", "start": 7920025, "end": 7954921}, {"filename": "/assets/adventurers/Assets/gltf/crossbow_2handed.gltf", "start": 7954921, "end": 7958004}, {"filename": "/assets/adventurers/Assets/gltf/dagger.bin", "start": 7958004, "end": 7965660}, {"filename": "/assets/adventurers/Assets/gltf/dagger.gltf", "start": 7965660, "end": 7968711}, {"filename": "/assets/adventurers/Assets/gltf/knight_texture.png", "start": 7968711, "end": 7981625}, {"filename": "/assets/adventurers/Assets/gltf/mage_texture.png", "start": 7981625, "end": 7996365}, {"filename": "/assets/adventurers/Assets/gltf/mug_empty.bin", "start": 7996365, "end": 8011009}, {"filename": "/assets/adventurers/Assets/gltf/mug_empty.gltf", "start": 8011009, "end": 8014084}, {"filename": "/assets/adventurers/Assets/gltf/mug_full.bin", "start": 8014084, "end": 8029856}, {"filename": "/assets/adventurers/Assets/gltf/mug_full.gltf", "start": 8029856, "end": 8032928}, {"filename": "/assets/adventurers/Assets/gltf/quiver.bin", "start": 8032928, "end": 8046812}, {"filename": "/assets/adventurers/Assets/gltf/quiver.gltf", "start": 8046812, "end": 8049865}, {"filename": "/assets/adventurers/Assets/gltf/ranger_texture.png", "start": 8049865, "end": 8063113}, {"filename": "/assets/adventurers/Assets/gltf/rogue_texture.png", "start": 8063113, "end": 8078139}, {"filename": "/assets/adventurers/Assets/gltf/shield_badge.bin", "start": 8078139, "end": 8088767}, {"filename": "/assets/adventurers/Assets/gltf/shield_badge.gltf", "start": 8088767, "end": 8091839}, {"filename": "/assets/adventurers/Assets/gltf/shield_badge_color.bin", "start": 8091839, "end": 8102339}, {"filename": "/assets/adventurers/Assets/gltf/shield_badge_color.gltf", "start": 8102339, "end": 8105429}, {"filename": "/assets/adventurers/Assets/gltf/shield_round.bin", "start": 8105429, "end": 8117437}, {"filename": "/assets/adventurers/Assets/gltf/shield_round.gltf", "start": 8117437, "end": 8120512}, {"filename": "/assets/adventurers/Assets/gltf/shield_round_barbarian.bin", "start": 8120512, "end": 8132264}, {"filename": "/assets/adventurers/Assets/gltf/shield_round_barbarian.gltf", "start": 8132264, "end": 8135378}, {"filename": "/assets/adventurers/Assets/gltf/shield_round_color.bin", "start": 8135378, "end": 8147130}, {"filename": "/assets/adventurers/Assets/gltf/shield_round_color.gltf", "start": 8147130, "end": 8150223}, {"filename": "/assets/adventurers/Assets/gltf/shield_spikes.bin", "start": 8150223, "end": 8171687}, {"filename": "/assets/adventurers/Assets/gltf/shield_spikes.gltf", "start": 8171687, "end": 8174763}, {"filename": "/assets/adventurers/Assets/gltf/shield_spikes_color.bin", "start": 8174763, "end": 8196227}, {"filename": "/assets/adventurers/Assets/gltf/shield_spikes_color.gltf", "start": 8196227, "end": 8199321}, {"filename": "/assets/adventurers/Assets/gltf/shield_square.bin", "start": 8199321, "end": 8210045}, {"filename": "/assets/adventurers/Assets/gltf/shield_square.gltf", "start": 8210045, "end": 8213119}, {"filename": "/assets/adventurers/Assets/gltf/shield_square_color.bin", "start": 8213119, "end": 8223843}, {"filename": "/assets/adventurers/Assets/gltf/shield_square_color.gltf", "start": 8223843, "end": 8226935}, {"filename": "/assets/adventurers/Assets/gltf/smokebomb.bin", "start": 8226935, "end": 8238395}, {"filename": "/assets/adventurers/Assets/gltf/smokebomb.gltf", "start": 8238395, "end": 8241457}, {"filename": "/assets/adventurers/Assets/gltf/spellbook_closed.bin", "start": 8241457, "end": 8255977}, {"filename": "/assets/adventurers/Assets/gltf/spellbook_closed.gltf", "start": 8255977, "end": 8259058}, {"filename": "/assets/adventurers/Assets/gltf/spellbook_open.bin", "start": 8259058, "end": 8274186}, {"filename": "/assets/adventurers/Assets/gltf/spellbook_open.gltf", "start": 8274186, "end": 8277258}, {"filename": "/assets/adventurers/Assets/gltf/staff.bin", "start": 8277258, "end": 8295834}, {"filename": "/assets/adventurers/Assets/gltf/staff.gltf", "start": 8295834, "end": 8298880}, {"filename": "/assets/adventurers/Assets/gltf/sword_1handed.bin", "start": 8298880, "end": 8312136}, {"filename": "/assets/adventurers/Assets/gltf/sword_1handed.gltf", "start": 8312136, "end": 8315211}, {"filename": "/assets/adventurers/Assets/gltf/sword_2handed.bin", "start": 8315211, "end": 8335123}, {"filename": "/assets/adventurers/Assets/gltf/sword_2handed.gltf", "start": 8335123, "end": 8338199}, {"filename": "/assets/adventurers/Assets/gltf/sword_2handed_color.bin", "start": 8338199, "end": 8358079}, {"filename": "/assets/adventurers/Assets/gltf/sword_2handed_color.gltf", "start": 8358079, "end": 8361173}, {"filename": "/assets/adventurers/Assets/gltf/wand.bin", "start": 8361173, "end": 8367129}, {"filename": "/assets/adventurers/Assets/gltf/wand.gltf", "start": 8367129, "end": 8370171}, {"filename": "/assets/adventurers/Assets/obj/arrow_bow.mtl", "start": 8370171, "end": 8370460}, {"filename": "/assets/adventurers/Assets/obj/arrow_bow.obj", "start": 8370460, "end": 8375266}, {"filename": "/assets/adventurers/Assets/obj/arrow_bow_bundle.mtl", "start": 8375266, "end": 8375555}, {"filename": "/assets/adventurers/Assets/obj/arrow_bow_bundle.obj", "start": 8375555, "end": 8402490}, {"filename": "/assets/adventurers/Assets/obj/arrow_crossbow.mtl", "start": 8402490, "end": 8402777}, {"filename": "/assets/adventurers/Assets/obj/arrow_crossbow.obj", "start": 8402777, "end": 8407529}, {"filename": "/assets/adventurers/Assets/obj/arrow_crossbow_bundle.mtl", "start": 8407529, "end": 8407816}, {"filename": "/assets/adventurers/Assets/obj/arrow_crossbow_bundle.obj", "start": 8407816, "end": 8434761}, {"filename": "/assets/adventurers/Assets/obj/axe_1handed.mtl", "start": 8434761, "end": 8435056}, {"filename": "/assets/adventurers/Assets/obj/axe_1handed.obj", "start": 8435056, "end": 8458242}, {"filename": "/assets/adventurers/Assets/obj/axe_2handed.mtl", "start": 8458242, "end": 8458537}, {"filename": "/assets/adventurers/Assets/obj/axe_2handed.obj", "start": 8458537, "end": 8505277}, {"filename": "/assets/adventurers/Assets/obj/barbarian_texture.png", "start": 8505277, "end": 8519701}, {"filename": "/assets/adventurers/Assets/obj/bow.mtl", "start": 8519701, "end": 8519990}, {"filename": "/assets/adventurers/Assets/obj/bow.obj", "start": 8519990, "end": 8551975}, {"filename": "/assets/adventurers/Assets/obj/bow_withString.mtl", "start": 8551975, "end": 8552264}, {"filename": "/assets/adventurers/Assets/obj/bow_withString.obj", "start": 8552264, "end": 8601549}, {"filename": "/assets/adventurers/Assets/obj/crossbow_1handed.mtl", "start": 8601549, "end": 8601836}, {"filename": "/assets/adventurers/Assets/obj/crossbow_1handed.obj", "start": 8601836, "end": 8650614}, {"filename": "/assets/adventurers/Assets/obj/crossbow_2handed.mtl", "start": 8650614, "end": 8650901}, {"filename": "/assets/adventurers/Assets/obj/crossbow_2handed.obj", "start": 8650901, "end": 8717441}, {"filename": "/assets/adventurers/Assets/obj/dagger.mtl", "start": 8717441, "end": 8717728}, {"filename": "/assets/adventurers/Assets/obj/dagger.obj", "start": 8717728, "end": 8732295}, {"filename": "/assets/adventurers/Assets/obj/knight_texture.png", "start": 8732295, "end": 8745209}, {"filename": "/assets/adventurers/Assets/obj/mage_texture.png", "start": 8745209, "end": 8759949}, {"filename": "/assets/adventurers/Assets/obj/mug_empty.mtl", "start": 8759949, "end": 8760244}, {"filename": "/assets/adventurers/Assets/obj/mug_empty.obj", "start": 8760244, "end": 8787824}, {"filename": "/assets/adventurers/Assets/obj/mug_full.mtl", "start": 8787824, "end": 8788119}, {"filename": "/assets/adventurers/Assets/obj/mug_full.obj", "start": 8788119, "end": 8819357}, {"filename": "/assets/adventurers/Assets/obj/quiver.mtl", "start": 8819357, "end": 8819644}, {"filename": "/assets/adventurers/Assets/obj/quiver.obj", "start": 8819644, "end": 8843151}, {"filename": "/assets/adventurers/Assets/obj/ranger_texture.png", "start": 8843151, "end": 8856399}, {"filename": "/assets/adventurers/Assets/obj/rogue_texture.png", "start": 8856399, "end": 8871425}, {"filename": "/assets/adventurers/Assets/obj/shield_badge.mtl", "start": 8871425, "end": 8871714}, {"filename": "/assets/adventurers/Assets/obj/shield_badge.obj", "start": 8871714, "end": 8893098}, {"filename": "/assets/adventurers/Assets/obj/shield_badge_color.mtl", "start": 8893098, "end": 8893387}, {"filename": "/assets/adventurers/Assets/obj/shield_badge_color.obj", "start": 8893387, "end": 8914699}, {"filename": "/assets/adventurers/Assets/obj/shield_round.mtl", "start": 8914699, "end": 8914988}, {"filename": "/assets/adventurers/Assets/obj/shield_round.obj", "start": 8914988, "end": 8938899}, {"filename": "/assets/adventurers/Assets/obj/shield_round_barbarian.mtl", "start": 8938899, "end": 8939194}, {"filename": "/assets/adventurers/Assets/obj/shield_round_barbarian.obj", "start": 8939194, "end": 8962966}, {"filename": "/assets/adventurers/Assets/obj/shield_round_color.mtl", "start": 8962966, "end": 8963255}, {"filename": "/assets/adventurers/Assets/obj/shield_round_color.obj", "start": 8963255, "end": 8987016}, {"filename": "/assets/adventurers/Assets/obj/shield_spikes.mtl", "start": 8987016, "end": 8987305}, {"filename": "/assets/adventurers/Assets/obj/shield_spikes.obj", "start": 8987305, "end": 9025045}, {"filename": "/assets/adventurers/Assets/obj/shield_spikes_color.mtl", "start": 9025045, "end": 9025334}, {"filename": "/assets/adventurers/Assets/obj/shield_spikes_color.obj", "start": 9025334, "end": 9063086}, {"filename": "/assets/adventurers/Assets/obj/shield_square.mtl", "start": 9063086, "end": 9063375}, {"filename": "/assets/adventurers/Assets/obj/shield_square.obj", "start": 9063375, "end": 9084984}, {"filename": "/assets/adventurers/Assets/obj/shield_square_color.mtl", "start": 9084984, "end": 9085273}, {"filename": "/assets/adventurers/Assets/obj/shield_square_color.obj", "start": 9085273, "end": 9106894}, {"filename": "/assets/adventurers/Assets/obj/smokebomb.mtl", "start": 9106894, "end": 9107181}, {"filename": "/assets/adventurers/Assets/obj/smokebomb.obj", "start": 9107181, "end": 9129625}, {"filename": "/assets/adventurers/Assets/obj/spellbook_closed.mtl", "start": 9129625, "end": 9129910}, {"filename": "/assets/adventurers/Assets/obj/spellbook_closed.obj", "start": 9129910, "end": 9153689}, {"filename": "/assets/adventurers/Assets/obj/spellbook_open.mtl", "start": 9153689, "end": 9153974}, {"filename": "/assets/adventurers/Assets/obj/spellbook_open.obj", "start": 9153974, "end": 9177887}, {"filename": "/assets/adventurers/Assets/obj/staff.mtl", "start": 9177887, "end": 9178172}, {"filename": "/assets/adventurers/Assets/obj/staff.obj", "start": 9178172, "end": 9213463}, {"filename": "/assets/adventurers/Assets/obj/sword_1handed.mtl", "start": 9213463, "end": 9213752}, {"filename": "/assets/adventurers/Assets/obj/sword_1handed.obj", "start": 9213752, "end": 9238014}, {"filename": "/assets/adventurers/Assets/obj/sword_2handed.mtl", "start": 9238014, "end": 9238303}, {"filename": "/assets/adventurers/Assets/obj/sword_2handed.obj", "start": 9238303, "end": 9272725}, {"filename": "/assets/adventurers/Assets/obj/sword_2handed_color.mtl", "start": 9272725, "end": 9273014}, {"filename": "/assets/adventurers/Assets/obj/sword_2handed_color.obj", "start": 9273014, "end": 9307448}, {"filename": "/assets/adventurers/Assets/obj/wand.mtl", "start": 9307448, "end": 9307733}, {"filename": "/assets/adventurers/Assets/obj/wand.obj", "start": 9307733, "end": 9319543}, {"filename": "/assets/adventurers/Characters/fbx/Barbarian.fbx", "start": 9319543, "end": 9801475}, {"filename": "/assets/adventurers/Characters/fbx/Knight.fbx", "start": 9801475, "end": 10287903}, {"filename": "/assets/adventurers/Characters/fbx/Mage.fbx", "start": 10287903, "end": 10778011}, {"filename": "/assets/adventurers/Characters/fbx/Ranger.fbx", "start": 10778011, "end": 11341559}, {"filename": "/assets/adventurers/Characters/fbx/Rogue.fbx", "start": 11341559, "end": 11840499}, {"filename": "/assets/adventurers/Characters/fbx/Rogue_Hooded.fbx", "start": 11840499, "end": 12355983}, {"filename": "/assets/adventurers/Characters/fbx/barbarian_texture.png", "start": 12355983, "end": 12370407}, {"filename": "/assets/adventurers/Characters/fbx/knight_texture.png", "start": 12370407, "end": 12383321}, {"filename": "/assets/adventurers/Characters/fbx/mage_texture.png", "start": 12383321, "end": 12398061}, {"filename": "/assets/adventurers/Characters/fbx/ranger_texture.png", "start": 12398061, "end": 12411309}, {"filename": "/assets/adventurers/Characters/fbx/rogue_texture.png", "start": 12411309, "end": 12426335}, {"filename": "/assets/adventurers/Characters/gltf/Barbarian.glb", "start": 12426335, "end": 12812983}, {"filename": "/assets/adventurers/Characters/gltf/Knight.glb", "start": 12812983, "end": 13154671}, {"filename": "/assets/adventurers/Characters/gltf/Mage.glb", "start": 13154671, "end": 13507143}, {"filename": "/assets/adventurers/Characters/gltf/Ranger.glb", "start": 13507143, "end": 13991855}, {"filename": "/assets/adventurers/Characters/gltf/Rogue.glb", "start": 13991855, "end": 14401043}, {"filename": "/assets/adventurers/Characters/gltf/Rogue_Hooded.glb", "start": 14401043, "end": 14782475}, {"filename": "/assets/adventurers/Characters/gltf/barbarian_texture.png", "start": 14782475, "end": 14796899}, {"filename": "/assets/adventurers/Characters/gltf/knight_texture.png", "start": 14796899, "end": 14809813}, {"filename": "/assets/adventurers/Characters/gltf/mage_texture.png", "start": 14809813, "end": 14824553}, {"filename": "/assets/adventurers/Characters/gltf/ranger_texture.png", "start": 14824553, "end": 14837801}, {"filename": "/assets/adventurers/Characters/gltf/rogue_texture.png", "start": 14837801, "end": 14852827}, {"filename": "/assets/adventurers/License.txt", "start": 14852827, "end": 14854381}, {"filename": "/assets/adventurers/More KayKit Assets.url", "start": 14854381, "end": 14854509}, {"filename": "/assets/adventurers/Patreon.url", "start": 14854509, "end": 14854634}, {"filename": "/assets/adventurers/Samples/barbarian.png", "start": 14854634, "end": 15489389}, {"filename": "/assets/adventurers/Samples/barbarian_Large.png", "start": 15489389, "end": 16438025}, {"filename": "/assets/adventurers/Samples/druid.png", "start": 16438025, "end": 17180482}, {"filename": "/assets/adventurers/Samples/engineer.png", "start": 17180482, "end": 17979299}, {"filename": "/assets/adventurers/Samples/knight.png", "start": 17979299, "end": 18785629}, {"filename": "/assets/adventurers/Samples/mage.png", "start": 18785629, "end": 19412056}, {"filename": "/assets/adventurers/Samples/ranger.png", "start": 19412056, "end": 20789132}, {"filename": "/assets/adventurers/Samples/rogue.png", "start": 20789132, "end": 21416601}, {"filename": "/assets/adventurers/Textures/barbarian_texture.png", "start": 21416601, "end": 21431025}, {"filename": "/assets/adventurers/Textures/knight_texture.png", "start": 21431025, "end": 21443939}, {"filename": "/assets/adventurers/Textures/mage_texture.png", "start": 21443939, "end": 21458679}, {"filename": "/assets/adventurers/Textures/ranger_texture.png", "start": 21458679, "end": 21471927}, {"filename": "/assets/adventurers/Textures/rogue_texture.png", "start": 21471927, "end": 21486953}, {"filename": "/assets/adventurers/contents.png", "start": 21486953, "end": 23027118}, {"filename": "/assets/ambience_flute.mp3", "start": 23027118, "end": 23384055, "audio": 1}, {"filename": "/assets/ambience_techno.mp3", "start": 23384055, "end": 23693344, "audio": 1}, {"filename": "/assets/bow.mp3", "start": 23693344, "end": 23758624, "audio": 1}, {"filename": "/assets/chest_open.wav", "start": 23758624, "end": 24629470, "audio": 1}, {"filename": "/assets/door.wav", "start": 24629470, "end": 24681694, "audio": 1}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_blue.fbx", "start": 24681694, "end": 24703642}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_brown.fbx", "start": 24703642, "end": 24725574}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_green.fbx", "start": 24725574, "end": 24747538}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_patternA_blue.fbx", "start": 24747538, "end": 24770030}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_patternA_brown.fbx", "start": 24770030, "end": 24792474}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_patternA_green.fbx", "start": 24792474, "end": 24814950}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_patternA_red.fbx", "start": 24814950, "end": 24837442}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_patternA_white.fbx", "start": 24837442, "end": 24859918}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_patternA_yellow.fbx", "start": 24859918, "end": 24882410}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_patternB_blue.fbx", "start": 24882410, "end": 24904486}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_patternB_brown.fbx", "start": 24904486, "end": 24926546}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_patternB_green.fbx", "start": 24926546, "end": 24948638}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_patternB_red.fbx", "start": 24948638, "end": 24970714}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_patternB_white.fbx", "start": 24970714, "end": 24992774}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_patternB_yellow.fbx", "start": 24992774, "end": 25014850}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_patternC_blue.fbx", "start": 25014850, "end": 25036926}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_patternC_brown.fbx", "start": 25036926, "end": 25059002}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_patternC_green.fbx", "start": 25059002, "end": 25081094}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_patternC_red.fbx", "start": 25081094, "end": 25103170}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_patternC_white.fbx", "start": 25103170, "end": 25125246}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_patternC_yellow.fbx", "start": 25125246, "end": 25147338}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_red.fbx", "start": 25147338, "end": 25169302}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_shield_blue.fbx", "start": 25169302, "end": 25211586}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_shield_brown.fbx", "start": 25211586, "end": 25253854}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_shield_green.fbx", "start": 25253854, "end": 25296138}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_shield_red.fbx", "start": 25296138, "end": 25338454}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_shield_white.fbx", "start": 25338454, "end": 25380738}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_shield_yellow.fbx", "start": 25380738, "end": 25423054}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_thin_blue.fbx", "start": 25423054, "end": 25444426}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_thin_brown.fbx", "start": 25444426, "end": 25465782}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_thin_green.fbx", "start": 25465782, "end": 25487170}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_thin_red.fbx", "start": 25487170, "end": 25508558}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_thin_white.fbx", "start": 25508558, "end": 25529930}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_thin_yellow.fbx", "start": 25529930, "end": 25551302}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_triple_blue.fbx", "start": 25551302, "end": 25577522}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_triple_brown.fbx", "start": 25577522, "end": 25603678}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_triple_green.fbx", "start": 25603678, "end": 25629930}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_triple_red.fbx", "start": 25629930, "end": 25656166}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_triple_white.fbx", "start": 25656166, "end": 25682338}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_triple_yellow.fbx", "start": 25682338, "end": 25708510}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_white.fbx", "start": 25708510, "end": 25730458}, {"filename": "/assets/dungeons/Assets/fbx(unity)/banner_yellow.fbx", "start": 25730458, "end": 25752406}, {"filename": "/assets/dungeons/Assets/fbx(unity)/barrel_large.fbx", "start": 25752406, "end": 25788882}, {"filename": "/assets/dungeons/Assets/fbx(unity)/barrel_large_decorated.fbx", "start": 25788882, "end": 25843902}, {"filename": "/assets/dungeons/Assets/fbx(unity)/barrel_small.fbx", "start": 25843902, "end": 25868650}, {"filename": "/assets/dungeons/Assets/fbx(unity)/barrel_small_stack.fbx", "start": 25868650, "end": 25922550}, {"filename": "/assets/dungeons/Assets/fbx(unity)/barrier.fbx", "start": 25922550, "end": 25943362}, {"filename": "/assets/dungeons/Assets/fbx(unity)/barrier_colum_half.fbx", "start": 25943362, "end": 25964446}, {"filename": "/assets/dungeons/Assets/fbx(unity)/barrier_column.fbx", "start": 25964446, "end": 25985802}, {"filename": "/assets/dungeons/Assets/fbx(unity)/barrier_corner.fbx", "start": 25985802, "end": 26008006}, {"filename": "/assets/dungeons/Assets/fbx(unity)/barrier_half.fbx", "start": 26008006, "end": 26027954}, {"filename": "/assets/dungeons/Assets/fbx(unity)/bed_decorated.fbx", "start": 26027954, "end": 26094062}, {"filename": "/assets/dungeons/Assets/fbx(unity)/bed_floor.fbx", "start": 26094062, "end": 26120138}, {"filename": "/assets/dungeons/Assets/fbx(unity)/bed_frame.fbx", "start": 26120138, "end": 26147974}, {"filename": "/assets/dungeons/Assets/fbx(unity)/bottle_A_brown.fbx", "start": 26147974, "end": 26171890}, {"filename": "/assets/dungeons/Assets/fbx(unity)/bottle_A_green.fbx", "start": 26171890, "end": 26195950}, {"filename": "/assets/dungeons/Assets/fbx(unity)/bottle_A_labeled_brown.fbx", "start": 26195950, "end": 26219946}, {"filename": "/assets/dungeons/Assets/fbx(unity)/bottle_A_labeled_green.fbx", "start": 26219946, "end": 26244054}, {"filename": "/assets/dungeons/Assets/fbx(unity)/bottle_B_brown.fbx", "start": 26244054, "end": 26267970}, {"filename": "/assets/dungeons/Assets/fbx(unity)/bottle_B_green.fbx", "start": 26267970, "end": 26292030}, {"filename": "/assets/dungeons/Assets/fbx(unity)/bottle_C_brown.fbx", "start": 26292030, "end": 26315962}, {"filename": "/assets/dungeons/Assets/fbx(unity)/bottle_C_green.fbx", "start": 26315962, "end": 26340038}, {"filename": "/assets/dungeons/Assets/fbx(unity)/box_large.fbx", "start": 26340038, "end": 26364258}, {"filename": "/assets/dungeons/Assets/fbx(unity)/box_small.fbx", "start": 26364258, "end": 26388478}, {"filename": "/assets/dungeons/Assets/fbx(unity)/box_small_decorated.fbx", "start": 26388478, "end": 26436922}, {"filename": "/assets/dungeons/Assets/fbx(unity)/box_stacked.fbx", "start": 26436922, "end": 26532198}, {"filename": "/assets/dungeons/Assets/fbx(unity)/candle.fbx", "start": 26532198, "end": 26553282}, {"filename": "/assets/dungeons/Assets/fbx(unity)/candle_lit.fbx", "start": 26553282, "end": 26575934}, {"filename": "/assets/dungeons/Assets/fbx(unity)/candle_melted.fbx", "start": 26575934, "end": 26597274}, {"filename": "/assets/dungeons/Assets/fbx(unity)/candle_thin.fbx", "start": 26597274, "end": 26618374}, {"filename": "/assets/dungeons/Assets/fbx(unity)/candle_thin_lit.fbx", "start": 26618374, "end": 26641058}, {"filename": "/assets/dungeons/Assets/fbx(unity)/candle_triple.fbx", "start": 26641058, "end": 26666718}, {"filename": "/assets/dungeons/Assets/fbx(unity)/ceiling_tile.fbx", "start": 26666718, "end": 26686794}, {"filename": "/assets/dungeons/Assets/fbx(unity)/chair.fbx", "start": 26686794, "end": 26715158}, {"filename": "/assets/dungeons/Assets/fbx(unity)/chest.fbx", "start": 26715158, "end": 26757906}, {"filename": "/assets/dungeons/Assets/fbx(unity)/chest_gold.fbx", "start": 26757906, "end": 26825854}, {"filename": "/assets/dungeons/Assets/fbx(unity)/coin.fbx", "start": 26825854, "end": 26847610}, {"filename": "/assets/dungeons/Assets/fbx(unity)/coin_stack_large.fbx", "start": 26847610, "end": 26916982}, {"filename": "/assets/dungeons/Assets/fbx(unity)/coin_stack_medium.fbx", "start": 26916982, "end": 26965042}, {"filename": "/assets/dungeons/Assets/fbx(unity)/coin_stack_small.fbx", "start": 26965042, "end": 26996878}, {"filename": "/assets/dungeons/Assets/fbx(unity)/column.fbx", "start": 26996878, "end": 27016810}, {"filename": "/assets/dungeons/Assets/fbx(unity)/crates_stacked.fbx", "start": 27016810, "end": 27081254}, {"filename": "/assets/dungeons/Assets/fbx(unity)/dungeon_texture.png", "start": 27081254, "end": 27098301}, {"filename": "/assets/dungeons/Assets/fbx(unity)/floor_dirt_large.fbx", "start": 27098301, "end": 27122073}, {"filename": "/assets/dungeons/Assets/fbx(unity)/floor_dirt_large_rocky.fbx", "start": 27122073, "end": 27150085}, {"filename": "/assets/dungeons/Assets/fbx(unity)/floor_dirt_small_A.fbx", "start": 27150085, "end": 27171041}, {"filename": "/assets/dungeons/Assets/fbx(unity)/floor_dirt_small_B.fbx", "start": 27171041, "end": 27191469}, {"filename": "/assets/dungeons/Assets/fbx(unity)/floor_dirt_small_C.fbx", "start": 27191469, "end": 27211801}, {"filename": "/assets/dungeons/Assets/fbx(unity)/floor_dirt_small_D.fbx", "start": 27211801, "end": 27231477}, {"filename": "/assets/dungeons/Assets/fbx(unity)/floor_dirt_small_corner.fbx", "start": 27231477, "end": 27252609}, {"filename": "/assets/dungeons/Assets/fbx(unity)/floor_dirt_small_weeds.fbx", "start": 27252609, "end": 27286733}, {"filename": "/assets/dungeons/Assets/fbx(unity)/floor_foundation_allsides.fbx", "start": 27286733, "end": 27316697}, {"filename": "/assets/dungeons/Assets/fbx(unity)/floor_foundation_corner.fbx", "start": 27316697, "end": 27342037}, {"filename": "/assets/dungeons/Assets/fbx(unity)/floor_foundation_diagonal_corner.fbx", "start": 27342037, "end": 27364961}, {"filename": "/assets/dungeons/Assets/fbx(unity)/floor_foundation_front.fbx", "start": 27364961, "end": 27387773}, {"filename": "/assets/dungeons/Assets/fbx(unity)/floor_foundation_front_and_back.fbx", "start": 27387773, "end": 27413129}, {"filename": "/assets/dungeons/Assets/fbx(unity)/floor_foundation_front_and_sides.fbx", "start": 27413129, "end": 27440773}, {"filename": "/assets/dungeons/Assets/fbx(unity)/floor_tile_big_grate.fbx", "start": 27440773, "end": 27467441}, {"filename": "/assets/dungeons/Assets/fbx(unity)/floor_tile_big_grate_open.fbx", "start": 27467441, "end": 27493773}, {"filename": "/assets/dungeons/Assets/fbx(unity)/floor_tile_big_spikes.fbx", "start": 27493773, "end": 27530777}, {"filename": "/assets/dungeons/Assets/fbx(unity)/floor_tile_extralarge_grates.fbx", "start": 27530777, "end": 27571557}, {"filename": "/assets/dungeons/Assets/fbx(unity)/floor_tile_extralarge_grates_open.fbx", "start": 27571557, "end": 27612177}, {"filename": "/assets/dungeons/Assets/fbx(unity)/floor_tile_grate.fbx", "start": 27612177, "end": 27634909}, {"filename": "/assets/dungeons/Assets/fbx(unity)/floor_tile_grate_open.fbx", "start": 27634909, "end": 27657609}, {"filename": "/assets/dungeons/Assets/fbx(unity)/floor_tile_large.fbx", "start": 27657609, "end": 27681221}, {"filename": "/assets/dungeons/Assets/fbx(unity)/floor_tile_large_rocks.fbx", "start": 27681221, "end": 27713473}, {"filename": "/assets/dungeons/Assets/fbx(unity)/floor_tile_small.fbx", "start": 27713473, "end": 27734125}, {"filename": "/assets/dungeons/Assets/fbx(unity)/floor_tile_small_broken_A.fbx", "start": 27734125, "end": 27756329}, {"filename": "/assets/dungeons/Assets/fbx(unity)/floor_tile_small_broken_B.fbx", "start": 27756329, "end": 27777749}, {"filename": "/assets/dungeons/Assets/fbx(unity)/floor_tile_small_corner.fbx", "start": 27777749, "end": 27797617}, {"filename": "/assets/dungeons/Assets/fbx(unity)/floor_tile_small_decorated.fbx", "start": 27797617, "end": 27830829}, {"filename": "/assets/dungeons/Assets/fbx(unity)/floor_tile_small_weeds_A.fbx", "start": 27830829, "end": 27870409}, {"filename": "/assets/dungeons/Assets/fbx(unity)/floor_tile_small_weeds_B.fbx", "start": 27870409, "end": 27905061}, {"filename": "/assets/dungeons/Assets/fbx(unity)/floor_wood_large.fbx", "start": 27905061, "end": 27929201}, {"filename": "/assets/dungeons/Assets/fbx(unity)/floor_wood_large_dark.fbx", "start": 27929201, "end": 27953341}, {"filename": "/assets/dungeons/Assets/fbx(unity)/floor_wood_small.fbx", "start": 27953341, "end": 27973913}, {"filename": "/assets/dungeons/Assets/fbx(unity)/floor_wood_small_dark.fbx", "start": 27973913, "end": 27994485}, {"filename": "/assets/dungeons/Assets/fbx(unity)/keg.fbx", "start": 27994485, "end": 28039025}, {"filename": "/assets/dungeons/Assets/fbx(unity)/keg_decorated.fbx", "start": 28039025, "end": 28126669}, {"filename": "/assets/dungeons/Assets/fbx(unity)/key.fbx", "start": 28126669, "end": 28150457}, {"filename": "/assets/dungeons/Assets/fbx(unity)/keyring.fbx", "start": 28150457, "end": 28192485}, {"filename": "/assets/dungeons/Assets/fbx(unity)/keyring_hanging.fbx", "start": 28192485, "end": 28234097}, {"filename": "/assets/dungeons/Assets/fbx(unity)/pillar.fbx", "start": 28234097, "end": 28256349}, {"filename": "/assets/dungeons/Assets/fbx(unity)/pillar_decorated.fbx", "start": 28256349, "end": 28301577}, {"filename": "/assets/dungeons/Assets/fbx(unity)/plate.fbx", "start": 28301577, "end": 28324309}, {"filename": "/assets/dungeons/Assets/fbx(unity)/plate_food_A.fbx", "start": 28324309, "end": 28362257}, {"filename": "/assets/dungeons/Assets/fbx(unity)/plate_food_B.fbx", "start": 28362257, "end": 28392253}, {"filename": "/assets/dungeons/Assets/fbx(unity)/plate_small.fbx", "start": 28392253, "end": 28415017}, {"filename": "/assets/dungeons/Assets/fbx(unity)/plate_stack.fbx", "start": 28415017, "end": 28446261}, {"filename": "/assets/dungeons/Assets/fbx(unity)/rubble_half.fbx", "start": 28446261, "end": 28482993}, {"filename": "/assets/dungeons/Assets/fbx(unity)/rubble_large.fbx", "start": 28482993, "end": 28537261}, {"filename": "/assets/dungeons/Assets/fbx(unity)/shelf_large.fbx", "start": 28537261, "end": 28558233}, {"filename": "/assets/dungeons/Assets/fbx(unity)/shelf_small.fbx", "start": 28558233, "end": 28578757}, {"filename": "/assets/dungeons/Assets/fbx(unity)/shelf_small_candles.fbx", "start": 28578757, "end": 28605329}, {"filename": "/assets/dungeons/Assets/fbx(unity)/shelves.fbx", "start": 28605329, "end": 28628125}, {"filename": "/assets/dungeons/Assets/fbx(unity)/stairs.fbx", "start": 28628125, "end": 28658905}, {"filename": "/assets/dungeons/Assets/fbx(unity)/stairs_long.fbx", "start": 28658905, "end": 28692277}, {"filename": "/assets/dungeons/Assets/fbx(unity)/stairs_long_modular_center.fbx", "start": 28692277, "end": 28712721}, {"filename": "/assets/dungeons/Assets/fbx(unity)/stairs_long_modular_left.fbx", "start": 28712721, "end": 28740157}, {"filename": "/assets/dungeons/Assets/fbx(unity)/stairs_long_modular_right.fbx", "start": 28740157, "end": 28767657}, {"filename": "/assets/dungeons/Assets/fbx(unity)/stairs_modular_center.fbx", "start": 28767657, "end": 28788085}, {"filename": "/assets/dungeons/Assets/fbx(unity)/stairs_modular_left.fbx", "start": 28788085, "end": 28814209}, {"filename": "/assets/dungeons/Assets/fbx(unity)/stairs_modular_right.fbx", "start": 28814209, "end": 28840317}, {"filename": "/assets/dungeons/Assets/fbx(unity)/stairs_narrow.fbx", "start": 28840317, "end": 28871049}, {"filename": "/assets/dungeons/Assets/fbx(unity)/stairs_wall_left.fbx", "start": 28871049, "end": 28909301}, {"filename": "/assets/dungeons/Assets/fbx(unity)/stairs_wall_right.fbx", "start": 28909301, "end": 28947649}, {"filename": "/assets/dungeons/Assets/fbx(unity)/stairs_walled.fbx", "start": 28947649, "end": 28992237}, {"filename": "/assets/dungeons/Assets/fbx(unity)/stairs_wide.fbx", "start": 28992237, "end": 29022985}, {"filename": "/assets/dungeons/Assets/fbx(unity)/stairs_wood.fbx", "start": 29022985, "end": 29049461}, {"filename": "/assets/dungeons/Assets/fbx(unity)/stairs_wood_decorated.fbx", "start": 29049461, "end": 29150417}, {"filename": "/assets/dungeons/Assets/fbx(unity)/stool.fbx", "start": 29150417, "end": 29174669}, {"filename": "/assets/dungeons/Assets/fbx(unity)/sword_shield.fbx", "start": 29174669, "end": 29213273}, {"filename": "/assets/dungeons/Assets/fbx(unity)/sword_shield_broken.fbx", "start": 29213273, "end": 29248853}, {"filename": "/assets/dungeons/Assets/fbx(unity)/sword_shield_gold.fbx", "start": 29248853, "end": 29287441}, {"filename": "/assets/dungeons/Assets/fbx(unity)/table_long.fbx", "start": 29287441, "end": 29314749}, {"filename": "/assets/dungeons/Assets/fbx(unity)/table_long_broken.fbx", "start": 29314749, "end": 29354393}, {"filename": "/assets/dungeons/Assets/fbx(unity)/table_long_decorated_A.fbx", "start": 29354393, "end": 29440469}, {"filename": "/assets/dungeons/Assets/fbx(unity)/table_long_decorated_C.fbx", "start": 29440469, "end": 29574433}, {"filename": "/assets/dungeons/Assets/fbx(unity)/table_long_tablecloth.fbx", "start": 29574433, "end": 29604013}, {"filename": "/assets/dungeons/Assets/fbx(unity)/table_long_tablecloth_decorated_A.fbx", "start": 29604013, "end": 29692457}, {"filename": "/assets/dungeons/Assets/fbx(unity)/table_medium.fbx", "start": 29692457, "end": 29718933}, {"filename": "/assets/dungeons/Assets/fbx(unity)/table_medium_broken.fbx", "start": 29718933, "end": 29747953}, {"filename": "/assets/dungeons/Assets/fbx(unity)/table_medium_decorated_A.fbx", "start": 29747953, "end": 29807341}, {"filename": "/assets/dungeons/Assets/fbx(unity)/table_medium_tablecloth.fbx", "start": 29807341, "end": 29834377}, {"filename": "/assets/dungeons/Assets/fbx(unity)/table_medium_tablecloth_decorated_B.fbx", "start": 29834377, "end": 29894437}, {"filename": "/assets/dungeons/Assets/fbx(unity)/table_small.fbx", "start": 29894437, "end": 29919233}, {"filename": "/assets/dungeons/Assets/fbx(unity)/table_small_decorated_A.fbx", "start": 29919233, "end": 29970413}, {"filename": "/assets/dungeons/Assets/fbx(unity)/table_small_decorated_B.fbx", "start": 29970413, "end": 30033865}, {"filename": "/assets/dungeons/Assets/fbx(unity)/torch.fbx", "start": 30033865, "end": 30059717}, {"filename": "/assets/dungeons/Assets/fbx(unity)/torch_lit.fbx", "start": 30059717, "end": 30087153}, {"filename": "/assets/dungeons/Assets/fbx(unity)/torch_mounted.fbx", "start": 30087153, "end": 30115613}, {"filename": "/assets/dungeons/Assets/fbx(unity)/trunk_large_A.fbx", "start": 30115613, "end": 30140921}, {"filename": "/assets/dungeons/Assets/fbx(unity)/trunk_large_B.fbx", "start": 30140921, "end": 30166229}, {"filename": "/assets/dungeons/Assets/fbx(unity)/trunk_large_C.fbx", "start": 30166229, "end": 30191569}, {"filename": "/assets/dungeons/Assets/fbx(unity)/trunk_medium_A.fbx", "start": 30191569, "end": 30215197}, {"filename": "/assets/dungeons/Assets/fbx(unity)/trunk_medium_B.fbx", "start": 30215197, "end": 30238825}, {"filename": "/assets/dungeons/Assets/fbx(unity)/trunk_medium_C.fbx", "start": 30238825, "end": 30262437}, {"filename": "/assets/dungeons/Assets/fbx(unity)/trunk_small_A.fbx", "start": 30262437, "end": 30287809}, {"filename": "/assets/dungeons/Assets/fbx(unity)/trunk_small_B.fbx", "start": 30287809, "end": 30313197}, {"filename": "/assets/dungeons/Assets/fbx(unity)/trunk_small_C.fbx", "start": 30313197, "end": 30338569}, {"filename": "/assets/dungeons/Assets/fbx(unity)/wall.fbx", "start": 30338569, "end": 30372997}, {"filename": "/assets/dungeons/Assets/fbx(unity)/wall_Tsplit.fbx", "start": 30372997, "end": 30411457}, {"filename": "/assets/dungeons/Assets/fbx(unity)/wall_Tsplit_sloped.fbx", "start": 30411457, "end": 30448989}, {"filename": "/assets/dungeons/Assets/fbx(unity)/wall_arched.fbx", "start": 30448989, "end": 30492345}, {"filename": "/assets/dungeons/Assets/fbx(unity)/wall_archedwindow_gated.fbx", "start": 30492345, "end": 30535029}, {"filename": "/assets/dungeons/Assets/fbx(unity)/wall_archedwindow_gated_scaffold.fbx", "start": 30535029, "end": 30575137}, {"filename": "/assets/dungeons/Assets/fbx(unity)/wall_archedwindow_open.fbx", "start": 30575137, "end": 30614973}, {"filename": "/assets/dungeons/Assets/fbx(unity)/wall_broken.fbx", "start": 30614973, "end": 30661081}, {"filename": "/assets/dungeons/Assets/fbx(unity)/wall_corner.fbx", "start": 30661081, "end": 30694901}, {"filename": "/assets/dungeons/Assets/fbx(unity)/wall_corner_gated.fbx", "start": 30694901, "end": 30730977}, {"filename": "/assets/dungeons/Assets/fbx(unity)/wall_corner_scaffold.fbx", "start": 30730977, "end": 30767549}, {"filename": "/assets/dungeons/Assets/fbx(unity)/wall_corner_small.fbx", "start": 30767549, "end": 30794425}, {"filename": "/assets/dungeons/Assets/fbx(unity)/wall_cracked.fbx", "start": 30794425, "end": 30883941}, {"filename": "/assets/dungeons/Assets/fbx(unity)/wall_crossing.fbx", "start": 30883941, "end": 30925489}, {"filename": "/assets/dungeons/Assets/fbx(unity)/wall_doorway.fbx", "start": 30925489, "end": 30978461}, {"filename": "/assets/dungeons/Assets/fbx(unity)/wall_doorway_Tsplit.fbx", "start": 30978461, "end": 31032793}, {"filename": "/assets/dungeons/Assets/fbx(unity)/wall_doorway_scaffold.fbx", "start": 31032793, "end": 31088325}, {"filename": "/assets/dungeons/Assets/fbx(unity)/wall_doorway_sides.fbx", "start": 31088325, "end": 31144545}, {"filename": "/assets/dungeons/Assets/fbx(unity)/wall_endcap.fbx", "start": 31144545, "end": 31170925}, {"filename": "/assets/dungeons/Assets/fbx(unity)/wall_gated.fbx", "start": 31170925, "end": 31203161}, {"filename": "/assets/dungeons/Assets/fbx(unity)/wall_half.fbx", "start": 31203161, "end": 31231493}, {"filename": "/assets/dungeons/Assets/fbx(unity)/wall_half_endcap.fbx", "start": 31231493, "end": 31259441}, {"filename": "/assets/dungeons/Assets/fbx(unity)/wall_half_endcap_sloped.fbx", "start": 31259441, "end": 31286205}, {"filename": "/assets/dungeons/Assets/fbx(unity)/wall_open_scaffold.fbx", "start": 31286205, "end": 31310265}, {"filename": "/assets/dungeons/Assets/fbx(unity)/wall_pillar.fbx", "start": 31310265, "end": 31346293}, {"filename": "/assets/dungeons/Assets/fbx(unity)/wall_scaffold.fbx", "start": 31346293, "end": 31384289}, {"filename": "/assets/dungeons/Assets/fbx(unity)/wall_shelves.fbx", "start": 31384289, "end": 31456909}, {"filename": "/assets/dungeons/Assets/fbx(unity)/wall_sloped.fbx", "start": 31456909, "end": 31484809}, {"filename": "/assets/dungeons/Assets/fbx(unity)/wall_window_closed.fbx", "start": 31484809, "end": 31524789}, {"filename": "/assets/dungeons/Assets/fbx(unity)/wall_window_closed_scaffold.fbx", "start": 31524789, "end": 31567393}, {"filename": "/assets/dungeons/Assets/fbx(unity)/wall_window_open.fbx", "start": 31567393, "end": 31605437}, {"filename": "/assets/dungeons/Assets/fbx(unity)/wall_window_open_scaffold.fbx", "start": 31605437, "end": 31646089}, {"filename": "/assets/dungeons/Assets/fbx/banner_blue.fbx", "start": 31646089, "end": 31668213}, {"filename": "/assets/dungeons/Assets/fbx/banner_brown.fbx", "start": 31668213, "end": 31690321}, {"filename": "/assets/dungeons/Assets/fbx/banner_green.fbx", "start": 31690321, "end": 31712461}, {"filename": "/assets/dungeons/Assets/fbx/banner_patternA_blue.fbx", "start": 31712461, "end": 31735145}, {"filename": "/assets/dungeons/Assets/fbx/banner_patternA_brown.fbx", "start": 31735145, "end": 31757781}, {"filename": "/assets/dungeons/Assets/fbx/banner_patternA_green.fbx", "start": 31757781, "end": 31780449}, {"filename": "/assets/dungeons/Assets/fbx/banner_patternA_red.fbx", "start": 31780449, "end": 31803117}, {"filename": "/assets/dungeons/Assets/fbx/banner_patternA_white.fbx", "start": 31803117, "end": 31825769}, {"filename": "/assets/dungeons/Assets/fbx/banner_patternA_yellow.fbx", "start": 31825769, "end": 31848437}, {"filename": "/assets/dungeons/Assets/fbx/banner_patternB_blue.fbx", "start": 31848437, "end": 31870689}, {"filename": "/assets/dungeons/Assets/fbx/banner_patternB_brown.fbx", "start": 31870689, "end": 31892925}, {"filename": "/assets/dungeons/Assets/fbx/banner_patternB_green.fbx", "start": 31892925, "end": 31915193}, {"filename": "/assets/dungeons/Assets/fbx/banner_patternB_red.fbx", "start": 31915193, "end": 31937445}, {"filename": "/assets/dungeons/Assets/fbx/banner_patternB_white.fbx", "start": 31937445, "end": 31959681}, {"filename": "/assets/dungeons/Assets/fbx/banner_patternB_yellow.fbx", "start": 31959681, "end": 31981933}, {"filename": "/assets/dungeons/Assets/fbx/banner_patternC_blue.fbx", "start": 31981933, "end": 32004185}, {"filename": "/assets/dungeons/Assets/fbx/banner_patternC_brown.fbx", "start": 32004185, "end": 32026437}, {"filename": "/assets/dungeons/Assets/fbx/banner_patternC_green.fbx", "start": 32026437, "end": 32048689}, {"filename": "/assets/dungeons/Assets/fbx/banner_patternC_red.fbx", "start": 32048689, "end": 32070941}, {"filename": "/assets/dungeons/Assets/fbx/banner_patternC_white.fbx", "start": 32070941, "end": 32093193}, {"filename": "/assets/dungeons/Assets/fbx/banner_patternC_yellow.fbx", "start": 32093193, "end": 32115445}, {"filename": "/assets/dungeons/Assets/fbx/banner_red.fbx", "start": 32115445, "end": 32137585}, {"filename": "/assets/dungeons/Assets/fbx/banner_shield_blue.fbx", "start": 32137585, "end": 32180013}, {"filename": "/assets/dungeons/Assets/fbx/banner_shield_brown.fbx", "start": 32180013, "end": 32222425}, {"filename": "/assets/dungeons/Assets/fbx/banner_shield_green.fbx", "start": 32222425, "end": 32264853}, {"filename": "/assets/dungeons/Assets/fbx/banner_shield_red.fbx", "start": 32264853, "end": 32307297}, {"filename": "/assets/dungeons/Assets/fbx/banner_shield_white.fbx", "start": 32307297, "end": 32349725}, {"filename": "/assets/dungeons/Assets/fbx/banner_shield_yellow.fbx", "start": 32349725, "end": 32392185}, {"filename": "/assets/dungeons/Assets/fbx/banner_thin_blue.fbx", "start": 32392185, "end": 32413717}, {"filename": "/assets/dungeons/Assets/fbx/banner_thin_brown.fbx", "start": 32413717, "end": 32435233}, {"filename": "/assets/dungeons/Assets/fbx/banner_thin_green.fbx", "start": 32435233, "end": 32456781}, {"filename": "/assets/dungeons/Assets/fbx/banner_thin_red.fbx", "start": 32456781, "end": 32478329}, {"filename": "/assets/dungeons/Assets/fbx/banner_thin_white.fbx", "start": 32478329, "end": 32499861}, {"filename": "/assets/dungeons/Assets/fbx/banner_thin_yellow.fbx", "start": 32499861, "end": 32521393}, {"filename": "/assets/dungeons/Assets/fbx/banner_triple_blue.fbx", "start": 32521393, "end": 32547789}, {"filename": "/assets/dungeons/Assets/fbx/banner_triple_brown.fbx", "start": 32547789, "end": 32574121}, {"filename": "/assets/dungeons/Assets/fbx/banner_triple_green.fbx", "start": 32574121, "end": 32600565}, {"filename": "/assets/dungeons/Assets/fbx/banner_triple_red.fbx", "start": 32600565, "end": 32626993}, {"filename": "/assets/dungeons/Assets/fbx/banner_triple_white.fbx", "start": 32626993, "end": 32653357}, {"filename": "/assets/dungeons/Assets/fbx/banner_triple_yellow.fbx", "start": 32653357, "end": 32679705}, {"filename": "/assets/dungeons/Assets/fbx/banner_white.fbx", "start": 32679705, "end": 32701829}, {"filename": "/assets/dungeons/Assets/fbx/banner_yellow.fbx", "start": 32701829, "end": 32723953}, {"filename": "/assets/dungeons/Assets/fbx/barrel_large.fbx", "start": 32723953, "end": 32760157}, {"filename": "/assets/dungeons/Assets/fbx/barrel_large_decorated.fbx", "start": 32760157, "end": 32814601}, {"filename": "/assets/dungeons/Assets/fbx/barrel_small.fbx", "start": 32814601, "end": 32839301}, {"filename": "/assets/dungeons/Assets/fbx/barrel_small_stack.fbx", "start": 32839301, "end": 32893185}, {"filename": "/assets/dungeons/Assets/fbx/barrier.fbx", "start": 32893185, "end": 32913917}, {"filename": "/assets/dungeons/Assets/fbx/barrier_colum_half.fbx", "start": 32913917, "end": 32934889}, {"filename": "/assets/dungeons/Assets/fbx/barrier_column.fbx", "start": 32934889, "end": 32956133}, {"filename": "/assets/dungeons/Assets/fbx/barrier_corner.fbx", "start": 32956133, "end": 32978081}, {"filename": "/assets/dungeons/Assets/fbx/barrier_half.fbx", "start": 32978081, "end": 32998013}, {"filename": "/assets/dungeons/Assets/fbx/bed_decorated.fbx", "start": 32998013, "end": 33062537}, {"filename": "/assets/dungeons/Assets/fbx/bed_floor.fbx", "start": 33062537, "end": 33088581}, {"filename": "/assets/dungeons/Assets/fbx/bed_frame.fbx", "start": 33088581, "end": 33116177}, {"filename": "/assets/dungeons/Assets/fbx/bottle_A_brown.fbx", "start": 33116177, "end": 33140093}, {"filename": "/assets/dungeons/Assets/fbx/bottle_A_green.fbx", "start": 33140093, "end": 33164137}, {"filename": "/assets/dungeons/Assets/fbx/bottle_A_labeled_brown.fbx", "start": 33164137, "end": 33188117}, {"filename": "/assets/dungeons/Assets/fbx/bottle_A_labeled_green.fbx", "start": 33188117, "end": 33212209}, {"filename": "/assets/dungeons/Assets/fbx/bottle_B_brown.fbx", "start": 33212209, "end": 33236141}, {"filename": "/assets/dungeons/Assets/fbx/bottle_B_green.fbx", "start": 33236141, "end": 33260201}, {"filename": "/assets/dungeons/Assets/fbx/bottle_C_brown.fbx", "start": 33260201, "end": 33284101}, {"filename": "/assets/dungeons/Assets/fbx/bottle_C_green.fbx", "start": 33284101, "end": 33308145}, {"filename": "/assets/dungeons/Assets/fbx/box_large.fbx", "start": 33308145, "end": 33332045}, {"filename": "/assets/dungeons/Assets/fbx/box_small.fbx", "start": 33332045, "end": 33355929}, {"filename": "/assets/dungeons/Assets/fbx/box_small_decorated.fbx", "start": 33355929, "end": 33403525}, {"filename": "/assets/dungeons/Assets/fbx/box_stacked.fbx", "start": 33403525, "end": 33496273}, {"filename": "/assets/dungeons/Assets/fbx/candle.fbx", "start": 33496273, "end": 33517261}, {"filename": "/assets/dungeons/Assets/fbx/candle_lit.fbx", "start": 33517261, "end": 33539817}, {"filename": "/assets/dungeons/Assets/fbx/candle_melted.fbx", "start": 33539817, "end": 33561125}, {"filename": "/assets/dungeons/Assets/fbx/candle_thin.fbx", "start": 33561125, "end": 33582145}, {"filename": "/assets/dungeons/Assets/fbx/candle_thin_lit.fbx", "start": 33582145, "end": 33604749}, {"filename": "/assets/dungeons/Assets/fbx/candle_triple.fbx", "start": 33604749, "end": 33629977}, {"filename": "/assets/dungeons/Assets/fbx/ceiling_tile.fbx", "start": 33629977, "end": 33650021}, {"filename": "/assets/dungeons/Assets/fbx/chair.fbx", "start": 33650021, "end": 33678129}, {"filename": "/assets/dungeons/Assets/fbx/chest.fbx", "start": 33678129, "end": 33720525}, {"filename": "/assets/dungeons/Assets/fbx/chest_gold.fbx", "start": 33720525, "end": 33787865}, {"filename": "/assets/dungeons/Assets/fbx/coin.fbx", "start": 33787865, "end": 33809573}, {"filename": "/assets/dungeons/Assets/fbx/coin_stack_large.fbx", "start": 33809573, "end": 33877265}, {"filename": "/assets/dungeons/Assets/fbx/coin_stack_medium.fbx", "start": 33877265, "end": 33924685}, {"filename": "/assets/dungeons/Assets/fbx/coin_stack_small.fbx", "start": 33924685, "end": 33956281}, {"filename": "/assets/dungeons/Assets/fbx/column.fbx", "start": 33956281, "end": 33976213}, {"filename": "/assets/dungeons/Assets/fbx/crates_stacked.fbx", "start": 33976213, "end": 34039249}, {"filename": "/assets/dungeons/Assets/fbx/dungeon_texture.png", "start": 34039249, "end": 34056296}, {"filename": "/assets/dungeons/Assets/fbx/floor_dirt_large.fbx", "start": 34056296, "end": 34079940}, {"filename": "/assets/dungeons/Assets/fbx/floor_dirt_large_rocky.fbx", "start": 34079940, "end": 34107744}, {"filename": "/assets/dungeons/Assets/fbx/floor_dirt_small_A.fbx", "start": 34107744, "end": 34128684}, {"filename": "/assets/dungeons/Assets/fbx/floor_dirt_small_B.fbx", "start": 34128684, "end": 34149128}, {"filename": "/assets/dungeons/Assets/fbx/floor_dirt_small_C.fbx", "start": 34149128, "end": 34169460}, {"filename": "/assets/dungeons/Assets/fbx/floor_dirt_small_D.fbx", "start": 34169460, "end": 34189168}, {"filename": "/assets/dungeons/Assets/fbx/floor_dirt_small_corner.fbx", "start": 34189168, "end": 34210188}, {"filename": "/assets/dungeons/Assets/fbx/floor_dirt_small_weeds.fbx", "start": 34210188, "end": 34244168}, {"filename": "/assets/dungeons/Assets/fbx/floor_foundation_allsides.fbx", "start": 34244168, "end": 34273716}, {"filename": "/assets/dungeons/Assets/fbx/floor_foundation_corner.fbx", "start": 34273716, "end": 34298832}, {"filename": "/assets/dungeons/Assets/fbx/floor_foundation_diagonal_corner.fbx", "start": 34298832, "end": 34321500}, {"filename": "/assets/dungeons/Assets/fbx/floor_foundation_front.fbx", "start": 34321500, "end": 34344120}, {"filename": "/assets/dungeons/Assets/fbx/floor_foundation_front_and_back.fbx", "start": 34344120, "end": 34369252}, {"filename": "/assets/dungeons/Assets/fbx/floor_foundation_front_and_sides.fbx", "start": 34369252, "end": 34396624}, {"filename": "/assets/dungeons/Assets/fbx/floor_tile_big_grate.fbx", "start": 34396624, "end": 34423068}, {"filename": "/assets/dungeons/Assets/fbx/floor_tile_big_grate_open.fbx", "start": 34423068, "end": 34449272}, {"filename": "/assets/dungeons/Assets/fbx/floor_tile_big_spikes.fbx", "start": 34449272, "end": 34485172}, {"filename": "/assets/dungeons/Assets/fbx/floor_tile_extralarge_grates.fbx", "start": 34485172, "end": 34525456}, {"filename": "/assets/dungeons/Assets/fbx/floor_tile_extralarge_grates_open.fbx", "start": 34525456, "end": 34565564}, {"filename": "/assets/dungeons/Assets/fbx/floor_tile_grate.fbx", "start": 34565564, "end": 34588312}, {"filename": "/assets/dungeons/Assets/fbx/floor_tile_grate_open.fbx", "start": 34588312, "end": 34611044}, {"filename": "/assets/dungeons/Assets/fbx/floor_tile_large.fbx", "start": 34611044, "end": 34634432}, {"filename": "/assets/dungeons/Assets/fbx/floor_tile_large_rocks.fbx", "start": 34634432, "end": 34666316}, {"filename": "/assets/dungeons/Assets/fbx/floor_tile_small.fbx", "start": 34666316, "end": 34686792}, {"filename": "/assets/dungeons/Assets/fbx/floor_tile_small_broken_A.fbx", "start": 34686792, "end": 34708740}, {"filename": "/assets/dungeons/Assets/fbx/floor_tile_small_broken_B.fbx", "start": 34708740, "end": 34729920}, {"filename": "/assets/dungeons/Assets/fbx/floor_tile_small_corner.fbx", "start": 34729920, "end": 34749724}, {"filename": "/assets/dungeons/Assets/fbx/floor_tile_small_decorated.fbx", "start": 34749724, "end": 34782184}, {"filename": "/assets/dungeons/Assets/fbx/floor_tile_small_weeds_A.fbx", "start": 34782184, "end": 34821412}, {"filename": "/assets/dungeons/Assets/fbx/floor_tile_small_weeds_B.fbx", "start": 34821412, "end": 34855712}, {"filename": "/assets/dungeons/Assets/fbx/floor_wood_large.fbx", "start": 34855712, "end": 34879788}, {"filename": "/assets/dungeons/Assets/fbx/floor_wood_large_dark.fbx", "start": 34879788, "end": 34903864}, {"filename": "/assets/dungeons/Assets/fbx/floor_wood_small.fbx", "start": 34903864, "end": 34924436}, {"filename": "/assets/dungeons/Assets/fbx/floor_wood_small_dark.fbx", "start": 34924436, "end": 34945024}, {"filename": "/assets/dungeons/Assets/fbx/keg.fbx", "start": 34945024, "end": 34989260}, {"filename": "/assets/dungeons/Assets/fbx/keg_decorated.fbx", "start": 34989260, "end": 35075448}, {"filename": "/assets/dungeons/Assets/fbx/key.fbx", "start": 35075448, "end": 35099268}, {"filename": "/assets/dungeons/Assets/fbx/keyring.fbx", "start": 35099268, "end": 35141280}, {"filename": "/assets/dungeons/Assets/fbx/keyring_hanging.fbx", "start": 35141280, "end": 35182988}, {"filename": "/assets/dungeons/Assets/fbx/pillar.fbx", "start": 35182988, "end": 35205048}, {"filename": "/assets/dungeons/Assets/fbx/pillar_decorated.fbx", "start": 35205048, "end": 35250212}, {"filename": "/assets/dungeons/Assets/fbx/plate.fbx", "start": 35250212, "end": 35272944}, {"filename": "/assets/dungeons/Assets/fbx/plate_food_A.fbx", "start": 35272944, "end": 35310748}, {"filename": "/assets/dungeons/Assets/fbx/plate_food_B.fbx", "start": 35310748, "end": 35340712}, {"filename": "/assets/dungeons/Assets/fbx/plate_small.fbx", "start": 35340712, "end": 35363476}, {"filename": "/assets/dungeons/Assets/fbx/plate_stack.fbx", "start": 35363476, "end": 35394704}, {"filename": "/assets/dungeons/Assets/fbx/rubble_half.fbx", "start": 35394704, "end": 35431404}, {"filename": "/assets/dungeons/Assets/fbx/rubble_large.fbx", "start": 35431404, "end": 35485624}, {"filename": "/assets/dungeons/Assets/fbx/shelf_large.fbx", "start": 35485624, "end": 35506484}, {"filename": "/assets/dungeons/Assets/fbx/shelf_small.fbx", "start": 35506484, "end": 35526960}, {"filename": "/assets/dungeons/Assets/fbx/shelf_small_candles.fbx", "start": 35526960, "end": 35553308}, {"filename": "/assets/dungeons/Assets/fbx/shelves.fbx", "start": 35553308, "end": 35575912}, {"filename": "/assets/dungeons/Assets/fbx/stairs.fbx", "start": 35575912, "end": 35606212}, {"filename": "/assets/dungeons/Assets/fbx/stairs_long.fbx", "start": 35606212, "end": 35639184}, {"filename": "/assets/dungeons/Assets/fbx/stairs_long_modular_center.fbx", "start": 35639184, "end": 35659564}, {"filename": "/assets/dungeons/Assets/fbx/stairs_long_modular_left.fbx", "start": 35659564, "end": 35686792}, {"filename": "/assets/dungeons/Assets/fbx/stairs_long_modular_right.fbx", "start": 35686792, "end": 35714132}, {"filename": "/assets/dungeons/Assets/fbx/stairs_modular_center.fbx", "start": 35714132, "end": 35734496}, {"filename": "/assets/dungeons/Assets/fbx/stairs_modular_left.fbx", "start": 35734496, "end": 35760412}, {"filename": "/assets/dungeons/Assets/fbx/stairs_modular_right.fbx", "start": 35760412, "end": 35786328}, {"filename": "/assets/dungeons/Assets/fbx/stairs_narrow.fbx", "start": 35786328, "end": 35816724}, {"filename": "/assets/dungeons/Assets/fbx/stairs_wall_left.fbx", "start": 35816724, "end": 35854512}, {"filename": "/assets/dungeons/Assets/fbx/stairs_wall_right.fbx", "start": 35854512, "end": 35892268}, {"filename": "/assets/dungeons/Assets/fbx/stairs_walled.fbx", "start": 35892268, "end": 35936056}, {"filename": "/assets/dungeons/Assets/fbx/stairs_wide.fbx", "start": 35936056, "end": 35966308}, {"filename": "/assets/dungeons/Assets/fbx/stairs_wood.fbx", "start": 35966308, "end": 35991760}, {"filename": "/assets/dungeons/Assets/fbx/stairs_wood_decorated.fbx", "start": 35991760, "end": 36090380}, {"filename": "/assets/dungeons/Assets/fbx/stool.fbx", "start": 36090380, "end": 36114376}, {"filename": "/assets/dungeons/Assets/fbx/sword_shield.fbx", "start": 36114376, "end": 36152932}, {"filename": "/assets/dungeons/Assets/fbx/sword_shield_broken.fbx", "start": 36152932, "end": 36188624}, {"filename": "/assets/dungeons/Assets/fbx/sword_shield_gold.fbx", "start": 36188624, "end": 36227164}, {"filename": "/assets/dungeons/Assets/fbx/table_long.fbx", "start": 36227164, "end": 36253960}, {"filename": "/assets/dungeons/Assets/fbx/table_long_broken.fbx", "start": 36253960, "end": 36293124}, {"filename": "/assets/dungeons/Assets/fbx/table_long_decorated_A.fbx", "start": 36293124, "end": 36378080}, {"filename": "/assets/dungeons/Assets/fbx/table_long_decorated_C.fbx", "start": 36378080, "end": 36510636}, {"filename": "/assets/dungeons/Assets/fbx/table_long_tablecloth.fbx", "start": 36510636, "end": 36539304}, {"filename": "/assets/dungeons/Assets/fbx/table_long_tablecloth_decorated_A.fbx", "start": 36539304, "end": 36626244}, {"filename": "/assets/dungeons/Assets/fbx/table_medium.fbx", "start": 36626244, "end": 36652192}, {"filename": "/assets/dungeons/Assets/fbx/table_medium_broken.fbx", "start": 36652192, "end": 36680972}, {"filename": "/assets/dungeons/Assets/fbx/table_medium_decorated_A.fbx", "start": 36680972, "end": 36739656}, {"filename": "/assets/dungeons/Assets/fbx/table_medium_tablecloth.fbx", "start": 36739656, "end": 36766100}, {"filename": "/assets/dungeons/Assets/fbx/table_medium_tablecloth_decorated_B.fbx", "start": 36766100, "end": 36825392}, {"filename": "/assets/dungeons/Assets/fbx/table_small.fbx", "start": 36825392, "end": 36849868}, {"filename": "/assets/dungeons/Assets/fbx/table_small_decorated_A.fbx", "start": 36849868, "end": 36900856}, {"filename": "/assets/dungeons/Assets/fbx/table_small_decorated_B.fbx", "start": 36900856, "end": 36962948}, {"filename": "/assets/dungeons/Assets/fbx/torch.fbx", "start": 36962948, "end": 36988768}, {"filename": "/assets/dungeons/Assets/fbx/torch_lit.fbx", "start": 36988768, "end": 37016156}, {"filename": "/assets/dungeons/Assets/fbx/torch_mounted.fbx", "start": 37016156, "end": 37044664}, {"filename": "/assets/dungeons/Assets/fbx/trunk_large_A.fbx", "start": 37044664, "end": 37069972}, {"filename": "/assets/dungeons/Assets/fbx/trunk_large_B.fbx", "start": 37069972, "end": 37095280}, {"filename": "/assets/dungeons/Assets/fbx/trunk_large_C.fbx", "start": 37095280, "end": 37120620}, {"filename": "/assets/dungeons/Assets/fbx/trunk_medium_A.fbx", "start": 37120620, "end": 37144216}, {"filename": "/assets/dungeons/Assets/fbx/trunk_medium_B.fbx", "start": 37144216, "end": 37167796}, {"filename": "/assets/dungeons/Assets/fbx/trunk_medium_C.fbx", "start": 37167796, "end": 37191376}, {"filename": "/assets/dungeons/Assets/fbx/trunk_small_A.fbx", "start": 37191376, "end": 37216684}, {"filename": "/assets/dungeons/Assets/fbx/trunk_small_B.fbx", "start": 37216684, "end": 37242008}, {"filename": "/assets/dungeons/Assets/fbx/trunk_small_C.fbx", "start": 37242008, "end": 37267316}, {"filename": "/assets/dungeons/Assets/fbx/wall.fbx", "start": 37267316, "end": 37301456}, {"filename": "/assets/dungeons/Assets/fbx/wall_Tsplit.fbx", "start": 37301456, "end": 37339452}, {"filename": "/assets/dungeons/Assets/fbx/wall_Tsplit_sloped.fbx", "start": 37339452, "end": 37376744}, {"filename": "/assets/dungeons/Assets/fbx/wall_arched.fbx", "start": 37376744, "end": 37419524}, {"filename": "/assets/dungeons/Assets/fbx/wall_archedwindow_gated.fbx", "start": 37419524, "end": 37461920}, {"filename": "/assets/dungeons/Assets/fbx/wall_archedwindow_gated_scaffold.fbx", "start": 37461920, "end": 37501564}, {"filename": "/assets/dungeons/Assets/fbx/wall_archedwindow_open.fbx", "start": 37501564, "end": 37540984}, {"filename": "/assets/dungeons/Assets/fbx/wall_broken.fbx", "start": 37540984, "end": 37586900}, {"filename": "/assets/dungeons/Assets/fbx/wall_corner.fbx", "start": 37586900, "end": 37620256}, {"filename": "/assets/dungeons/Assets/fbx/wall_corner_gated.fbx", "start": 37620256, "end": 37655628}, {"filename": "/assets/dungeons/Assets/fbx/wall_corner_scaffold.fbx", "start": 37655628, "end": 37691640}, {"filename": "/assets/dungeons/Assets/fbx/wall_corner_small.fbx", "start": 37691640, "end": 37718308}, {"filename": "/assets/dungeons/Assets/fbx/wall_cracked.fbx", "start": 37718308, "end": 37806384}, {"filename": "/assets/dungeons/Assets/fbx/wall_crossing.fbx", "start": 37806384, "end": 37847404}, {"filename": "/assets/dungeons/Assets/fbx/wall_doorway.fbx", "start": 37847404, "end": 37899288}, {"filename": "/assets/dungeons/Assets/fbx/wall_doorway_Tsplit.fbx", "start": 37899288, "end": 37952644}, {"filename": "/assets/dungeons/Assets/fbx/wall_doorway_scaffold.fbx", "start": 37952644, "end": 38006928}, {"filename": "/assets/dungeons/Assets/fbx/wall_doorway_sides.fbx", "start": 38006928, "end": 38061292}, {"filename": "/assets/dungeons/Assets/fbx/wall_endcap.fbx", "start": 38061292, "end": 38087480}, {"filename": "/assets/dungeons/Assets/fbx/wall_gated.fbx", "start": 38087480, "end": 38119460}, {"filename": "/assets/dungeons/Assets/fbx/wall_half.fbx", "start": 38119460, "end": 38147392}, {"filename": "/assets/dungeons/Assets/fbx/wall_half_endcap.fbx", "start": 38147392, "end": 38175132}, {"filename": "/assets/dungeons/Assets/fbx/wall_half_endcap_sloped.fbx", "start": 38175132, "end": 38201560}, {"filename": "/assets/dungeons/Assets/fbx/wall_open_scaffold.fbx", "start": 38201560, "end": 38225460}, {"filename": "/assets/dungeons/Assets/fbx/wall_pillar.fbx", "start": 38225460, "end": 38261168}, {"filename": "/assets/dungeons/Assets/fbx/wall_scaffold.fbx", "start": 38261168, "end": 38298636}, {"filename": "/assets/dungeons/Assets/fbx/wall_shelves.fbx", "start": 38298636, "end": 38370056}, {"filename": "/assets/dungeons/Assets/fbx/wall_sloped.fbx", "start": 38370056, "end": 38397604}, {"filename": "/assets/dungeons/Assets/fbx/wall_window_closed.fbx", "start": 38397604, "end": 38437200}, {"filename": "/assets/dungeons/Assets/fbx/wall_window_closed_scaffold.fbx", "start": 38437200, "end": 38479276}, {"filename": "/assets/dungeons/Assets/fbx/wall_window_open.fbx", "start": 38479276, "end": 38516968}, {"filename": "/assets/dungeons/Assets/fbx/wall_window_open_scaffold.fbx", "start": 38516968, "end": 38557156}, {"filename": "/assets/dungeons/Assets/gltf/banner_blue.bin", "start": 38557156, "end": 38563724}, {"filename": "/assets/dungeons/Assets/gltf/banner_blue.gltf", "start": 38563724, "end": 38566770}, {"filename": "/assets/dungeons/Assets/gltf/banner_brown.bin", "start": 38566770, "end": 38573338}, {"filename": "/assets/dungeons/Assets/gltf/banner_brown.gltf", "start": 38573338, "end": 38576387}, {"filename": "/assets/dungeons/Assets/gltf/banner_green.bin", "start": 38576387, "end": 38582955}, {"filename": "/assets/dungeons/Assets/gltf/banner_green.gltf", "start": 38582955, "end": 38586004}, {"filename": "/assets/dungeons/Assets/gltf/banner_patternA_blue.bin", "start": 38586004, "end": 38593704}, {"filename": "/assets/dungeons/Assets/gltf/banner_patternA_blue.gltf", "start": 38593704, "end": 38596777}, {"filename": "/assets/dungeons/Assets/gltf/banner_patternA_brown.bin", "start": 38596777, "end": 38604477}, {"filename": "/assets/dungeons/Assets/gltf/banner_patternA_brown.gltf", "start": 38604477, "end": 38607553}, {"filename": "/assets/dungeons/Assets/gltf/banner_patternA_green.bin", "start": 38607553, "end": 38615253}, {"filename": "/assets/dungeons/Assets/gltf/banner_patternA_green.gltf", "start": 38615253, "end": 38618329}, {"filename": "/assets/dungeons/Assets/gltf/banner_patternA_red.bin", "start": 38618329, "end": 38626029}, {"filename": "/assets/dungeons/Assets/gltf/banner_patternA_red.gltf", "start": 38626029, "end": 38629099}, {"filename": "/assets/dungeons/Assets/gltf/banner_patternA_white.bin", "start": 38629099, "end": 38636799}, {"filename": "/assets/dungeons/Assets/gltf/banner_patternA_white.gltf", "start": 38636799, "end": 38639875}, {"filename": "/assets/dungeons/Assets/gltf/banner_patternA_yellow.bin", "start": 38639875, "end": 38647575}, {"filename": "/assets/dungeons/Assets/gltf/banner_patternA_yellow.gltf", "start": 38647575, "end": 38650654}, {"filename": "/assets/dungeons/Assets/gltf/banner_patternB_blue.bin", "start": 38650654, "end": 38657542}, {"filename": "/assets/dungeons/Assets/gltf/banner_patternB_blue.gltf", "start": 38657542, "end": 38660615}, {"filename": "/assets/dungeons/Assets/gltf/banner_patternB_brown.bin", "start": 38660615, "end": 38667503}, {"filename": "/assets/dungeons/Assets/gltf/banner_patternB_brown.gltf", "start": 38667503, "end": 38670579}, {"filename": "/assets/dungeons/Assets/gltf/banner_patternB_green.bin", "start": 38670579, "end": 38677467}, {"filename": "/assets/dungeons/Assets/gltf/banner_patternB_green.gltf", "start": 38677467, "end": 38680543}, {"filename": "/assets/dungeons/Assets/gltf/banner_patternB_red.bin", "start": 38680543, "end": 38687431}, {"filename": "/assets/dungeons/Assets/gltf/banner_patternB_red.gltf", "start": 38687431, "end": 38690501}, {"filename": "/assets/dungeons/Assets/gltf/banner_patternB_white.bin", "start": 38690501, "end": 38697389}, {"filename": "/assets/dungeons/Assets/gltf/banner_patternB_white.gltf", "start": 38697389, "end": 38700465}, {"filename": "/assets/dungeons/Assets/gltf/banner_patternB_yellow.bin", "start": 38700465, "end": 38707353}, {"filename": "/assets/dungeons/Assets/gltf/banner_patternB_yellow.gltf", "start": 38707353, "end": 38710432}, {"filename": "/assets/dungeons/Assets/gltf/banner_patternC_blue.bin", "start": 38710432, "end": 38717320}, {"filename": "/assets/dungeons/Assets/gltf/banner_patternC_blue.gltf", "start": 38717320, "end": 38720393}, {"filename": "/assets/dungeons/Assets/gltf/banner_patternC_brown.bin", "start": 38720393, "end": 38727281}, {"filename": "/assets/dungeons/Assets/gltf/banner_patternC_brown.gltf", "start": 38727281, "end": 38730357}, {"filename": "/assets/dungeons/Assets/gltf/banner_patternC_green.bin", "start": 38730357, "end": 38737245}, {"filename": "/assets/dungeons/Assets/gltf/banner_patternC_green.gltf", "start": 38737245, "end": 38740321}, {"filename": "/assets/dungeons/Assets/gltf/banner_patternC_red.bin", "start": 38740321, "end": 38747209}, {"filename": "/assets/dungeons/Assets/gltf/banner_patternC_red.gltf", "start": 38747209, "end": 38750279}, {"filename": "/assets/dungeons/Assets/gltf/banner_patternC_white.bin", "start": 38750279, "end": 38757167}, {"filename": "/assets/dungeons/Assets/gltf/banner_patternC_white.gltf", "start": 38757167, "end": 38760243}, {"filename": "/assets/dungeons/Assets/gltf/banner_patternC_yellow.bin", "start": 38760243, "end": 38767131}, {"filename": "/assets/dungeons/Assets/gltf/banner_patternC_yellow.gltf", "start": 38767131, "end": 38770210}, {"filename": "/assets/dungeons/Assets/gltf/banner_red.bin", "start": 38770210, "end": 38776778}, {"filename": "/assets/dungeons/Assets/gltf/banner_red.gltf", "start": 38776778, "end": 38779821}, {"filename": "/assets/dungeons/Assets/gltf/banner_shield_blue.bin", "start": 38779821, "end": 38816361}, {"filename": "/assets/dungeons/Assets/gltf/banner_shield_blue.gltf", "start": 38816361, "end": 38819440}, {"filename": "/assets/dungeons/Assets/gltf/banner_shield_brown.bin", "start": 38819440, "end": 38855980}, {"filename": "/assets/dungeons/Assets/gltf/banner_shield_brown.gltf", "start": 38855980, "end": 38859062}, {"filename": "/assets/dungeons/Assets/gltf/banner_shield_green.bin", "start": 38859062, "end": 38895602}, {"filename": "/assets/dungeons/Assets/gltf/banner_shield_green.gltf", "start": 38895602, "end": 38898684}, {"filename": "/assets/dungeons/Assets/gltf/banner_shield_red.bin", "start": 38898684, "end": 38935224}, {"filename": "/assets/dungeons/Assets/gltf/banner_shield_red.gltf", "start": 38935224, "end": 38938300}, {"filename": "/assets/dungeons/Assets/gltf/banner_shield_white.bin", "start": 38938300, "end": 38974840}, {"filename": "/assets/dungeons/Assets/gltf/banner_shield_white.gltf", "start": 38974840, "end": 38977922}, {"filename": "/assets/dungeons/Assets/gltf/banner_shield_yellow.bin", "start": 38977922, "end": 39014462}, {"filename": "/assets/dungeons/Assets/gltf/banner_shield_yellow.gltf", "start": 39014462, "end": 39017547}, {"filename": "/assets/dungeons/Assets/gltf/banner_thin_blue.bin", "start": 39017547, "end": 39023007}, {"filename": "/assets/dungeons/Assets/gltf/banner_thin_blue.gltf", "start": 39023007, "end": 39026069}, {"filename": "/assets/dungeons/Assets/gltf/banner_thin_brown.bin", "start": 39026069, "end": 39031529}, {"filename": "/assets/dungeons/Assets/gltf/banner_thin_brown.gltf", "start": 39031529, "end": 39034594}, {"filename": "/assets/dungeons/Assets/gltf/banner_thin_green.bin", "start": 39034594, "end": 39040054}, {"filename": "/assets/dungeons/Assets/gltf/banner_thin_green.gltf", "start": 39040054, "end": 39043119}, {"filename": "/assets/dungeons/Assets/gltf/banner_thin_red.bin", "start": 39043119, "end": 39048579}, {"filename": "/assets/dungeons/Assets/gltf/banner_thin_red.gltf", "start": 39048579, "end": 39051638}, {"filename": "/assets/dungeons/Assets/gltf/banner_thin_white.bin", "start": 39051638, "end": 39057098}, {"filename": "/assets/dungeons/Assets/gltf/banner_thin_white.gltf", "start": 39057098, "end": 39060163}, {"filename": "/assets/dungeons/Assets/gltf/banner_thin_yellow.bin", "start": 39060163, "end": 39065623}, {"filename": "/assets/dungeons/Assets/gltf/banner_thin_yellow.gltf", "start": 39065623, "end": 39068691}, {"filename": "/assets/dungeons/Assets/gltf/banner_triple_blue.bin", "start": 39068691, "end": 39086175}, {"filename": "/assets/dungeons/Assets/gltf/banner_triple_blue.gltf", "start": 39086175, "end": 39089246}, {"filename": "/assets/dungeons/Assets/gltf/banner_triple_brown.bin", "start": 39089246, "end": 39106730}, {"filename": "/assets/dungeons/Assets/gltf/banner_triple_brown.gltf", "start": 39106730, "end": 39109804}, {"filename": "/assets/dungeons/Assets/gltf/banner_triple_green.bin", "start": 39109804, "end": 39127288}, {"filename": "/assets/dungeons/Assets/gltf/banner_triple_green.gltf", "start": 39127288, "end": 39130362}, {"filename": "/assets/dungeons/Assets/gltf/banner_triple_red.bin", "start": 39130362, "end": 39147846}, {"filename": "/assets/dungeons/Assets/gltf/banner_triple_red.gltf", "start": 39147846, "end": 39150914}, {"filename": "/assets/dungeons/Assets/gltf/banner_triple_white.bin", "start": 39150914, "end": 39168398}, {"filename": "/assets/dungeons/Assets/gltf/banner_triple_white.gltf", "start": 39168398, "end": 39171472}, {"filename": "/assets/dungeons/Assets/gltf/banner_triple_yellow.bin", "start": 39171472, "end": 39188956}, {"filename": "/assets/dungeons/Assets/gltf/banner_triple_yellow.gltf", "start": 39188956, "end": 39192033}, {"filename": "/assets/dungeons/Assets/gltf/banner_white.bin", "start": 39192033, "end": 39198601}, {"filename": "/assets/dungeons/Assets/gltf/banner_white.gltf", "start": 39198601, "end": 39201650}, {"filename": "/assets/dungeons/Assets/gltf/banner_yellow.bin", "start": 39201650, "end": 39208218}, {"filename": "/assets/dungeons/Assets/gltf/banner_yellow.gltf", "start": 39208218, "end": 39211270}, {"filename": "/assets/dungeons/Assets/gltf/barrel_large.bin", "start": 39211270, "end": 39237038}, {"filename": "/assets/dungeons/Assets/gltf/barrel_large.gltf", "start": 39237038, "end": 39240095}, {"filename": "/assets/dungeons/Assets/gltf/barrel_large_decorated.bin", "start": 39240095, "end": 39291275}, {"filename": "/assets/dungeons/Assets/gltf/barrel_large_decorated.gltf", "start": 39291275, "end": 39294370}, {"filename": "/assets/dungeons/Assets/gltf/barrel_small.bin", "start": 39294370, "end": 39308926}, {"filename": "/assets/dungeons/Assets/gltf/barrel_small.gltf", "start": 39308926, "end": 39311985}, {"filename": "/assets/dungeons/Assets/gltf/barrel_small_stack.bin", "start": 39311985, "end": 39385505}, {"filename": "/assets/dungeons/Assets/gltf/barrel_small_stack.gltf", "start": 39385505, "end": 39388574}, {"filename": "/assets/dungeons/Assets/gltf/barrier.bin", "start": 39388574, "end": 39394874}, {"filename": "/assets/dungeons/Assets/gltf/barrier.gltf", "start": 39394874, "end": 39397830}, {"filename": "/assets/dungeons/Assets/gltf/barrier_colum_half.bin", "start": 39397830, "end": 39403978}, {"filename": "/assets/dungeons/Assets/gltf/barrier_colum_half.gltf", "start": 39403978, "end": 39407013}, {"filename": "/assets/dungeons/Assets/gltf/barrier_column.bin", "start": 39407013, "end": 39414841}, {"filename": "/assets/dungeons/Assets/gltf/barrier_column.gltf", "start": 39414841, "end": 39417847}, {"filename": "/assets/dungeons/Assets/gltf/barrier_corner.bin", "start": 39417847, "end": 39426935}, {"filename": "/assets/dungeons/Assets/gltf/barrier_corner.gltf", "start": 39426935, "end": 39429941}, {"filename": "/assets/dungeons/Assets/gltf/barrier_half.bin", "start": 39429941, "end": 39432881}, {"filename": "/assets/dungeons/Assets/gltf/barrier_half.gltf", "start": 39432881, "end": 39435862}, {"filename": "/assets/dungeons/Assets/gltf/bed_decorated.bin", "start": 39435862, "end": 39505954}, {"filename": "/assets/dungeons/Assets/gltf/bed_decorated.gltf", "start": 39505954, "end": 39508972}, {"filename": "/assets/dungeons/Assets/gltf/bed_floor.bin", "start": 39508972, "end": 39516508}, {"filename": "/assets/dungeons/Assets/gltf/bed_floor.gltf", "start": 39516508, "end": 39519476}, {"filename": "/assets/dungeons/Assets/gltf/bed_frame.bin", "start": 39519476, "end": 39532116}, {"filename": "/assets/dungeons/Assets/gltf/bed_frame.gltf", "start": 39532116, "end": 39535106}, {"filename": "/assets/dungeons/Assets/gltf/bottle_A_brown.bin", "start": 39535106, "end": 39542850}, {"filename": "/assets/dungeons/Assets/gltf/bottle_A_brown.gltf", "start": 39542850, "end": 39545912}, {"filename": "/assets/dungeons/Assets/gltf/bottle_A_green.bin", "start": 39545912, "end": 39554168}, {"filename": "/assets/dungeons/Assets/gltf/bottle_A_green.gltf", "start": 39554168, "end": 39557230}, {"filename": "/assets/dungeons/Assets/gltf/bottle_A_labeled_brown.bin", "start": 39557230, "end": 39565102}, {"filename": "/assets/dungeons/Assets/gltf/bottle_A_labeled_brown.gltf", "start": 39565102, "end": 39568188}, {"filename": "/assets/dungeons/Assets/gltf/bottle_A_labeled_green.bin", "start": 39568188, "end": 39576572}, {"filename": "/assets/dungeons/Assets/gltf/bottle_A_labeled_green.gltf", "start": 39576572, "end": 39579658}, {"filename": "/assets/dungeons/Assets/gltf/bottle_B_brown.bin", "start": 39579658, "end": 39587402}, {"filename": "/assets/dungeons/Assets/gltf/bottle_B_brown.gltf", "start": 39587402, "end": 39590462}, {"filename": "/assets/dungeons/Assets/gltf/bottle_B_green.bin", "start": 39590462, "end": 39598718}, {"filename": "/assets/dungeons/Assets/gltf/bottle_B_green.gltf", "start": 39598718, "end": 39601778}, {"filename": "/assets/dungeons/Assets/gltf/bottle_C_brown.bin", "start": 39601778, "end": 39609522}, {"filename": "/assets/dungeons/Assets/gltf/bottle_C_brown.gltf", "start": 39609522, "end": 39612585}, {"filename": "/assets/dungeons/Assets/gltf/bottle_C_green.bin", "start": 39612585, "end": 39620841}, {"filename": "/assets/dungeons/Assets/gltf/bottle_C_green.gltf", "start": 39620841, "end": 39623904}, {"filename": "/assets/dungeons/Assets/gltf/box_large.bin", "start": 39623904, "end": 39635272}, {"filename": "/assets/dungeons/Assets/gltf/box_large.gltf", "start": 39635272, "end": 39638291}, {"filename": "/assets/dungeons/Assets/gltf/box_small.bin", "start": 39638291, "end": 39649659}, {"filename": "/assets/dungeons/Assets/gltf/box_small.gltf", "start": 39649659, "end": 39652631}, {"filename": "/assets/dungeons/Assets/gltf/box_small_decorated.bin", "start": 39652631, "end": 39706903}, {"filename": "/assets/dungeons/Assets/gltf/box_small_decorated.gltf", "start": 39706903, "end": 39709991}, {"filename": "/assets/dungeons/Assets/gltf/box_stacked.bin", "start": 39709991, "end": 39833431}, {"filename": "/assets/dungeons/Assets/gltf/box_stacked.gltf", "start": 39833431, "end": 39836496}, {"filename": "/assets/dungeons/Assets/gltf/candle.bin", "start": 39836496, "end": 39839804}, {"filename": "/assets/dungeons/Assets/gltf/candle.gltf", "start": 39839804, "end": 39842819}, {"filename": "/assets/dungeons/Assets/gltf/candle_lit.bin", "start": 39842819, "end": 39847379}, {"filename": "/assets/dungeons/Assets/gltf/candle_lit.gltf", "start": 39847379, "end": 39850409}, {"filename": "/assets/dungeons/Assets/gltf/candle_melted.bin", "start": 39850409, "end": 39853717}, {"filename": "/assets/dungeons/Assets/gltf/candle_melted.gltf", "start": 39853717, "end": 39856753}, {"filename": "/assets/dungeons/Assets/gltf/candle_thin.bin", "start": 39856753, "end": 39860061}, {"filename": "/assets/dungeons/Assets/gltf/candle_thin.gltf", "start": 39860061, "end": 39863092}, {"filename": "/assets/dungeons/Assets/gltf/candle_thin_lit.bin", "start": 39863092, "end": 39867652}, {"filename": "/assets/dungeons/Assets/gltf/candle_thin_lit.gltf", "start": 39867652, "end": 39870698}, {"filename": "/assets/dungeons/Assets/gltf/candle_triple.bin", "start": 39870698, "end": 39880622}, {"filename": "/assets/dungeons/Assets/gltf/candle_triple.gltf", "start": 39880622, "end": 39883661}, {"filename": "/assets/dungeons/Assets/gltf/ceiling_tile.bin", "start": 39883661, "end": 39887253}, {"filename": "/assets/dungeons/Assets/gltf/ceiling_tile.gltf", "start": 39887253, "end": 39890237}, {"filename": "/assets/dungeons/Assets/gltf/chair.bin", "start": 39890237, "end": 39908193}, {"filename": "/assets/dungeons/Assets/gltf/chair.gltf", "start": 39908193, "end": 39911232}, {"filename": "/assets/dungeons/Assets/gltf/chest.bin", "start": 39911232, "end": 39951728}, {"filename": "/assets/dungeons/Assets/gltf/chest.gltf", "start": 39951728, "end": 39956839}, {"filename": "/assets/dungeons/Assets/gltf/chest_gold.bin", "start": 39956839, "end": 40037447}, {"filename": "/assets/dungeons/Assets/gltf/chest_gold.gltf", "start": 40037447, "end": 40042590}, {"filename": "/assets/dungeons/Assets/gltf/coin.bin", "start": 40042590, "end": 40047038}, {"filename": "/assets/dungeons/Assets/gltf/coin.gltf", "start": 40047038, "end": 40050068}, {"filename": "/assets/dungeons/Assets/gltf/coin_stack_large.bin", "start": 40050068, "end": 40132532}, {"filename": "/assets/dungeons/Assets/gltf/coin_stack_large.gltf", "start": 40132532, "end": 40135591}, {"filename": "/assets/dungeons/Assets/gltf/coin_stack_medium.bin", "start": 40135591, "end": 40181591}, {"filename": "/assets/dungeons/Assets/gltf/coin_stack_medium.gltf", "start": 40181591, "end": 40184674}, {"filename": "/assets/dungeons/Assets/gltf/coin_stack_small.bin", "start": 40184674, "end": 40205474}, {"filename": "/assets/dungeons/Assets/gltf/coin_stack_small.gltf", "start": 40205474, "end": 40208547}, {"filename": "/assets/dungeons/Assets/gltf/column.bin", "start": 40208547, "end": 40211755}, {"filename": "/assets/dungeons/Assets/gltf/column.gltf", "start": 40211755, "end": 40214767}, {"filename": "/assets/dungeons/Assets/gltf/crates_stacked.bin", "start": 40214767, "end": 40296835}, {"filename": "/assets/dungeons/Assets/gltf/crates_stacked.gltf", "start": 40296835, "end": 40299905}, {"filename": "/assets/dungeons/Assets/gltf/dungeon_texture.png", "start": 40299905, "end": 40316952}, {"filename": "/assets/dungeons/Assets/gltf/floor_dirt_large.bin", "start": 40316952, "end": 40323848}, {"filename": "/assets/dungeons/Assets/gltf/floor_dirt_large.gltf", "start": 40323848, "end": 40326912}, {"filename": "/assets/dungeons/Assets/gltf/floor_dirt_large_rocky.bin", "start": 40326912, "end": 40338916}, {"filename": "/assets/dungeons/Assets/gltf/floor_dirt_large_rocky.gltf", "start": 40338916, "end": 40342000}, {"filename": "/assets/dungeons/Assets/gltf/floor_dirt_small_A.bin", "start": 40342000, "end": 40345152}, {"filename": "/assets/dungeons/Assets/gltf/floor_dirt_small_A.gltf", "start": 40345152, "end": 40348220}, {"filename": "/assets/dungeons/Assets/gltf/floor_dirt_small_B.bin", "start": 40348220, "end": 40350420}, {"filename": "/assets/dungeons/Assets/gltf/floor_dirt_small_B.gltf", "start": 40350420, "end": 40353484}, {"filename": "/assets/dungeons/Assets/gltf/floor_dirt_small_C.bin", "start": 40353484, "end": 40355764}, {"filename": "/assets/dungeons/Assets/gltf/floor_dirt_small_C.gltf", "start": 40355764, "end": 40358827}, {"filename": "/assets/dungeons/Assets/gltf/floor_dirt_small_D.bin", "start": 40358827, "end": 40360195}, {"filename": "/assets/dungeons/Assets/gltf/floor_dirt_small_D.gltf", "start": 40360195, "end": 40363258}, {"filename": "/assets/dungeons/Assets/gltf/floor_dirt_small_corner.bin", "start": 40363258, "end": 40366890}, {"filename": "/assets/dungeons/Assets/gltf/floor_dirt_small_corner.gltf", "start": 40366890, "end": 40369941}, {"filename": "/assets/dungeons/Assets/gltf/floor_dirt_small_weeds.bin", "start": 40369941, "end": 40389525}, {"filename": "/assets/dungeons/Assets/gltf/floor_dirt_small_weeds.gltf", "start": 40389525, "end": 40392614}, {"filename": "/assets/dungeons/Assets/gltf/floor_foundation_allsides.bin", "start": 40392614, "end": 40419734}, {"filename": "/assets/dungeons/Assets/gltf/floor_foundation_allsides.gltf", "start": 40419734, "end": 40422791}, {"filename": "/assets/dungeons/Assets/gltf/floor_foundation_corner.bin", "start": 40422791, "end": 40437379}, {"filename": "/assets/dungeons/Assets/gltf/floor_foundation_corner.gltf", "start": 40437379, "end": 40440396}, {"filename": "/assets/dungeons/Assets/gltf/floor_foundation_diagonal_corner.bin", "start": 40440396, "end": 40448200}, {"filename": "/assets/dungeons/Assets/gltf/floor_foundation_diagonal_corner.gltf", "start": 40448200, "end": 40451241}, {"filename": "/assets/dungeons/Assets/gltf/floor_foundation_front.bin", "start": 40451241, "end": 40459565}, {"filename": "/assets/dungeons/Assets/gltf/floor_foundation_front.gltf", "start": 40459565, "end": 40462560}, {"filename": "/assets/dungeons/Assets/gltf/floor_foundation_front_and_back.bin", "start": 40462560, "end": 40477148}, {"filename": "/assets/dungeons/Assets/gltf/floor_foundation_front_and_back.gltf", "start": 40477148, "end": 40480189}, {"filename": "/assets/dungeons/Assets/gltf/floor_foundation_front_and_sides.bin", "start": 40480189, "end": 40501045}, {"filename": "/assets/dungeons/Assets/gltf/floor_foundation_front_and_sides.gltf", "start": 40501045, "end": 40504106}, {"filename": "/assets/dungeons/Assets/gltf/floor_tile_big_grate.bin", "start": 40504106, "end": 40523394}, {"filename": "/assets/dungeons/Assets/gltf/floor_tile_big_grate.gltf", "start": 40523394, "end": 40526390}, {"filename": "/assets/dungeons/Assets/gltf/floor_tile_big_grate_open.bin", "start": 40526390, "end": 40545118}, {"filename": "/assets/dungeons/Assets/gltf/floor_tile_big_grate_open.gltf", "start": 40545118, "end": 40548129}, {"filename": "/assets/dungeons/Assets/gltf/floor_tile_big_spikes.bin", "start": 40548129, "end": 40589269}, {"filename": "/assets/dungeons/Assets/gltf/floor_tile_big_spikes.gltf", "start": 40589269, "end": 40594196}, {"filename": "/assets/dungeons/Assets/gltf/floor_tile_extralarge_grates.bin", "start": 40594196, "end": 40641964}, {"filename": "/assets/dungeons/Assets/gltf/floor_tile_extralarge_grates.gltf", "start": 40641964, "end": 40645009}, {"filename": "/assets/dungeons/Assets/gltf/floor_tile_extralarge_grates_open.bin", "start": 40645009, "end": 40692217}, {"filename": "/assets/dungeons/Assets/gltf/floor_tile_extralarge_grates_open.gltf", "start": 40692217, "end": 40695277}, {"filename": "/assets/dungeons/Assets/gltf/floor_tile_grate.bin", "start": 40695277, "end": 40704961}, {"filename": "/assets/dungeons/Assets/gltf/floor_tile_grate.gltf", "start": 40704961, "end": 40707958}, {"filename": "/assets/dungeons/Assets/gltf/floor_tile_grate_open.bin", "start": 40707958, "end": 40717502}, {"filename": "/assets/dungeons/Assets/gltf/floor_tile_grate_open.gltf", "start": 40717502, "end": 40720514}, {"filename": "/assets/dungeons/Assets/gltf/floor_tile_large.bin", "start": 40720514, "end": 40730442}, {"filename": "/assets/dungeons/Assets/gltf/floor_tile_large.gltf", "start": 40730442, "end": 40733441}, {"filename": "/assets/dungeons/Assets/gltf/floor_tile_large_rocks.bin", "start": 40733441, "end": 40753005}, {"filename": "/assets/dungeons/Assets/gltf/floor_tile_large_rocks.gltf", "start": 40753005, "end": 40756025}, {"filename": "/assets/dungeons/Assets/gltf/floor_tile_small.bin", "start": 40756025, "end": 40759621}, {"filename": "/assets/dungeons/Assets/gltf/floor_tile_small.gltf", "start": 40759621, "end": 40762618}, {"filename": "/assets/dungeons/Assets/gltf/floor_tile_small_broken_A.bin", "start": 40762618, "end": 40767726}, {"filename": "/assets/dungeons/Assets/gltf/floor_tile_small_broken_A.gltf", "start": 40767726, "end": 40770751}, {"filename": "/assets/dungeons/Assets/gltf/floor_tile_small_broken_B.bin", "start": 40770751, "end": 40774791}, {"filename": "/assets/dungeons/Assets/gltf/floor_tile_small_broken_B.gltf", "start": 40774791, "end": 40777815}, {"filename": "/assets/dungeons/Assets/gltf/floor_tile_small_corner.bin", "start": 40777815, "end": 40779667}, {"filename": "/assets/dungeons/Assets/gltf/floor_tile_small_corner.gltf", "start": 40779667, "end": 40782678}, {"filename": "/assets/dungeons/Assets/gltf/floor_tile_small_decorated.bin", "start": 40782678, "end": 40804346}, {"filename": "/assets/dungeons/Assets/gltf/floor_tile_small_decorated.gltf", "start": 40804346, "end": 40807378}, {"filename": "/assets/dungeons/Assets/gltf/floor_tile_small_weeds_A.bin", "start": 40807378, "end": 40834370}, {"filename": "/assets/dungeons/Assets/gltf/floor_tile_small_weeds_A.gltf", "start": 40834370, "end": 40837397}, {"filename": "/assets/dungeons/Assets/gltf/floor_tile_small_weeds_B.bin", "start": 40837397, "end": 40857873}, {"filename": "/assets/dungeons/Assets/gltf/floor_tile_small_weeds_B.gltf", "start": 40857873, "end": 40860900}, {"filename": "/assets/dungeons/Assets/gltf/floor_wood_large.bin", "start": 40860900, "end": 40879428}, {"filename": "/assets/dungeons/Assets/gltf/floor_wood_large.gltf", "start": 40879428, "end": 40882430}, {"filename": "/assets/dungeons/Assets/gltf/floor_wood_large_dark.bin", "start": 40882430, "end": 40900958}, {"filename": "/assets/dungeons/Assets/gltf/floor_wood_large_dark.gltf", "start": 40900958, "end": 40903975}, {"filename": "/assets/dungeons/Assets/gltf/floor_wood_small.bin", "start": 40903975, "end": 40909115}, {"filename": "/assets/dungeons/Assets/gltf/floor_wood_small.gltf", "start": 40909115, "end": 40912113}, {"filename": "/assets/dungeons/Assets/gltf/floor_wood_small_dark.bin", "start": 40912113, "end": 40917253}, {"filename": "/assets/dungeons/Assets/gltf/floor_wood_small_dark.gltf", "start": 40917253, "end": 40920266}, {"filename": "/assets/dungeons/Assets/gltf/keg.bin", "start": 40920266, "end": 40959926}, {"filename": "/assets/dungeons/Assets/gltf/keg.gltf", "start": 40959926, "end": 40962964}, {"filename": "/assets/dungeons/Assets/gltf/keg_decorated.bin", "start": 40962964, "end": 41078256}, {"filename": "/assets/dungeons/Assets/gltf/keg_decorated.gltf", "start": 41078256, "end": 41081327}, {"filename": "/assets/dungeons/Assets/gltf/key.bin", "start": 41081327, "end": 41087407}, {"filename": "/assets/dungeons/Assets/gltf/key.gltf", "start": 41087407, "end": 41090434}, {"filename": "/assets/dungeons/Assets/gltf/keyring.bin", "start": 41090434, "end": 41117650}, {"filename": "/assets/dungeons/Assets/gltf/keyring.gltf", "start": 41117650, "end": 41120695}, {"filename": "/assets/dungeons/Assets/gltf/keyring_hanging.bin", "start": 41120695, "end": 41147911}, {"filename": "/assets/dungeons/Assets/gltf/keyring_hanging.gltf", "start": 41147911, "end": 41150980}, {"filename": "/assets/dungeons/Assets/gltf/pillar.bin", "start": 41150980, "end": 41159508}, {"filename": "/assets/dungeons/Assets/gltf/pillar.gltf", "start": 41159508, "end": 41162493}, {"filename": "/assets/dungeons/Assets/gltf/pillar_decorated.bin", "start": 41162493, "end": 41206453}, {"filename": "/assets/dungeons/Assets/gltf/pillar_decorated.gltf", "start": 41206453, "end": 41209492}, {"filename": "/assets/dungeons/Assets/gltf/plate.bin", "start": 41209492, "end": 41214788}, {"filename": "/assets/dungeons/Assets/gltf/plate.gltf", "start": 41214788, "end": 41217823}, {"filename": "/assets/dungeons/Assets/gltf/plate_food_A.bin", "start": 41217823, "end": 41238963}, {"filename": "/assets/dungeons/Assets/gltf/plate_food_A.gltf", "start": 41238963, "end": 41242023}, {"filename": "/assets/dungeons/Assets/gltf/plate_food_B.bin", "start": 41242023, "end": 41254547}, {"filename": "/assets/dungeons/Assets/gltf/plate_food_B.gltf", "start": 41254547, "end": 41257605}, {"filename": "/assets/dungeons/Assets/gltf/plate_small.bin", "start": 41257605, "end": 41262901}, {"filename": "/assets/dungeons/Assets/gltf/plate_small.gltf", "start": 41262901, "end": 41265955}, {"filename": "/assets/dungeons/Assets/gltf/plate_stack.bin", "start": 41265955, "end": 41281779}, {"filename": "/assets/dungeons/Assets/gltf/plate_stack.gltf", "start": 41281779, "end": 41284834}, {"filename": "/assets/dungeons/Assets/gltf/rubble_half.bin", "start": 41284834, "end": 41306598}, {"filename": "/assets/dungeons/Assets/gltf/rubble_half.gltf", "start": 41306598, "end": 41309643}, {"filename": "/assets/dungeons/Assets/gltf/rubble_large.bin", "start": 41309643, "end": 41352771}, {"filename": "/assets/dungeons/Assets/gltf/rubble_large.gltf", "start": 41352771, "end": 41355813}, {"filename": "/assets/dungeons/Assets/gltf/shelf_large.bin", "start": 41355813, "end": 41360813}, {"filename": "/assets/dungeons/Assets/gltf/shelf_large.gltf", "start": 41360813, "end": 41363797}, {"filename": "/assets/dungeons/Assets/gltf/shelf_small.bin", "start": 41363797, "end": 41367677}, {"filename": "/assets/dungeons/Assets/gltf/shelf_small.gltf", "start": 41367677, "end": 41370701}, {"filename": "/assets/dungeons/Assets/gltf/shelf_small_candles.bin", "start": 41370701, "end": 41382617}, {"filename": "/assets/dungeons/Assets/gltf/shelf_small_candles.gltf", "start": 41382617, "end": 41385668}, {"filename": "/assets/dungeons/Assets/gltf/shelves.bin", "start": 41385668, "end": 41395668}, {"filename": "/assets/dungeons/Assets/gltf/shelves.gltf", "start": 41395668, "end": 41398628}, {"filename": "/assets/dungeons/Assets/gltf/stairs.bin", "start": 41398628, "end": 41424328}, {"filename": "/assets/dungeons/Assets/gltf/stairs.gltf", "start": 41424328, "end": 41427318}, {"filename": "/assets/dungeons/Assets/gltf/stairs_long.bin", "start": 41427318, "end": 41459666}, {"filename": "/assets/dungeons/Assets/gltf/stairs_long.gltf", "start": 41459666, "end": 41462673}, {"filename": "/assets/dungeons/Assets/gltf/stairs_long_modular_center.bin", "start": 41462673, "end": 41466389}, {"filename": "/assets/dungeons/Assets/gltf/stairs_long_modular_center.gltf", "start": 41466389, "end": 41469395}, {"filename": "/assets/dungeons/Assets/gltf/stairs_long_modular_left.bin", "start": 41469395, "end": 41487427}, {"filename": "/assets/dungeons/Assets/gltf/stairs_long_modular_left.gltf", "start": 41487427, "end": 41490485}, {"filename": "/assets/dungeons/Assets/gltf/stairs_long_modular_right.bin", "start": 41490485, "end": 41508517}, {"filename": "/assets/dungeons/Assets/gltf/stairs_long_modular_right.gltf", "start": 41508517, "end": 41511525}, {"filename": "/assets/dungeons/Assets/gltf/stairs_modular_center.bin", "start": 41511525, "end": 41515241}, {"filename": "/assets/dungeons/Assets/gltf/stairs_modular_center.gltf", "start": 41515241, "end": 41518232}, {"filename": "/assets/dungeons/Assets/gltf/stairs_modular_left.bin", "start": 41518232, "end": 41532940}, {"filename": "/assets/dungeons/Assets/gltf/stairs_modular_left.gltf", "start": 41532940, "end": 41535983}, {"filename": "/assets/dungeons/Assets/gltf/stairs_modular_right.bin", "start": 41535983, "end": 41550691}, {"filename": "/assets/dungeons/Assets/gltf/stairs_modular_right.gltf", "start": 41550691, "end": 41553683}, {"filename": "/assets/dungeons/Assets/gltf/stairs_narrow.bin", "start": 41553683, "end": 41579383}, {"filename": "/assets/dungeons/Assets/gltf/stairs_narrow.gltf", "start": 41579383, "end": 41582392}, {"filename": "/assets/dungeons/Assets/gltf/stairs_wall_left.bin", "start": 41582392, "end": 41622028}, {"filename": "/assets/dungeons/Assets/gltf/stairs_wall_left.gltf", "start": 41622028, "end": 41625020}, {"filename": "/assets/dungeons/Assets/gltf/stairs_wall_right.bin", "start": 41625020, "end": 41664656}, {"filename": "/assets/dungeons/Assets/gltf/stairs_wall_right.gltf", "start": 41664656, "end": 41667651}, {"filename": "/assets/dungeons/Assets/gltf/stairs_walled.bin", "start": 41667651, "end": 41721223}, {"filename": "/assets/dungeons/Assets/gltf/stairs_walled.gltf", "start": 41721223, "end": 41724225}, {"filename": "/assets/dungeons/Assets/gltf/stairs_wide.bin", "start": 41724225, "end": 41749925}, {"filename": "/assets/dungeons/Assets/gltf/stairs_wide.gltf", "start": 41749925, "end": 41752930}, {"filename": "/assets/dungeons/Assets/gltf/stairs_wood.bin", "start": 41752930, "end": 41770450}, {"filename": "/assets/dungeons/Assets/gltf/stairs_wood.gltf", "start": 41770450, "end": 41773467}, {"filename": "/assets/dungeons/Assets/gltf/stairs_wood_decorated.bin", "start": 41773467, "end": 41912787}, {"filename": "/assets/dungeons/Assets/gltf/stairs_wood_decorated.gltf", "start": 41912787, "end": 41915847}, {"filename": "/assets/dungeons/Assets/gltf/stool.bin", "start": 41915847, "end": 41926735}, {"filename": "/assets/dungeons/Assets/gltf/stool.gltf", "start": 41926735, "end": 41929756}, {"filename": "/assets/dungeons/Assets/gltf/sword_shield.bin", "start": 41929756, "end": 41958640}, {"filename": "/assets/dungeons/Assets/gltf/sword_shield.gltf", "start": 41958640, "end": 41961698}, {"filename": "/assets/dungeons/Assets/gltf/sword_shield_broken.bin", "start": 41961698, "end": 41983434}, {"filename": "/assets/dungeons/Assets/gltf/sword_shield_broken.gltf", "start": 41983434, "end": 41986513}, {"filename": "/assets/dungeons/Assets/gltf/sword_shield_gold.bin", "start": 41986513, "end": 42015397}, {"filename": "/assets/dungeons/Assets/gltf/sword_shield_gold.gltf", "start": 42015397, "end": 42018470}, {"filename": "/assets/dungeons/Assets/gltf/table_long.bin", "start": 42018470, "end": 42035382}, {"filename": "/assets/dungeons/Assets/gltf/table_long.gltf", "start": 42035382, "end": 42038381}, {"filename": "/assets/dungeons/Assets/gltf/table_long_broken.bin", "start": 42038381, "end": 42073617}, {"filename": "/assets/dungeons/Assets/gltf/table_long_broken.gltf", "start": 42073617, "end": 42076673}, {"filename": "/assets/dungeons/Assets/gltf/table_long_decorated_A.bin", "start": 42076673, "end": 42167801}, {"filename": "/assets/dungeons/Assets/gltf/table_long_decorated_A.gltf", "start": 42167801, "end": 42170880}, {"filename": "/assets/dungeons/Assets/gltf/table_long_decorated_C.bin", "start": 42170880, "end": 42362148}, {"filename": "/assets/dungeons/Assets/gltf/table_long_decorated_C.gltf", "start": 42362148, "end": 42365248}, {"filename": "/assets/dungeons/Assets/gltf/table_long_tablecloth.bin", "start": 42365248, "end": 42385548}, {"filename": "/assets/dungeons/Assets/gltf/table_long_tablecloth.gltf", "start": 42385548, "end": 42388582}, {"filename": "/assets/dungeons/Assets/gltf/table_long_tablecloth_decorated_A.bin", "start": 42388582, "end": 42483098}, {"filename": "/assets/dungeons/Assets/gltf/table_long_tablecloth_decorated_A.gltf", "start": 42483098, "end": 42486210}, {"filename": "/assets/dungeons/Assets/gltf/table_medium.bin", "start": 42486210, "end": 42501826}, {"filename": "/assets/dungeons/Assets/gltf/table_medium.gltf", "start": 42501826, "end": 42504832}, {"filename": "/assets/dungeons/Assets/gltf/table_medium_broken.bin", "start": 42504832, "end": 42521580}, {"filename": "/assets/dungeons/Assets/gltf/table_medium_broken.gltf", "start": 42521580, "end": 42524658}, {"filename": "/assets/dungeons/Assets/gltf/table_medium_decorated_A.bin", "start": 42524658, "end": 42582206}, {"filename": "/assets/dungeons/Assets/gltf/table_medium_decorated_A.gltf", "start": 42582206, "end": 42585274}, {"filename": "/assets/dungeons/Assets/gltf/table_medium_tablecloth.bin", "start": 42585274, "end": 42602318}, {"filename": "/assets/dungeons/Assets/gltf/table_medium_tablecloth.gltf", "start": 42602318, "end": 42605374}, {"filename": "/assets/dungeons/Assets/gltf/table_medium_tablecloth_decorated_B.bin", "start": 42605374, "end": 42664350}, {"filename": "/assets/dungeons/Assets/gltf/table_medium_tablecloth_decorated_B.gltf", "start": 42664350, "end": 42667468}, {"filename": "/assets/dungeons/Assets/gltf/table_small.bin", "start": 42667468, "end": 42679756}, {"filename": "/assets/dungeons/Assets/gltf/table_small.gltf", "start": 42679756, "end": 42682773}, {"filename": "/assets/dungeons/Assets/gltf/table_small_decorated_A.bin", "start": 42682773, "end": 42729969}, {"filename": "/assets/dungeons/Assets/gltf/table_small_decorated_A.gltf", "start": 42729969, "end": 42733048}, {"filename": "/assets/dungeons/Assets/gltf/table_small_decorated_B.bin", "start": 42733048, "end": 42819560}, {"filename": "/assets/dungeons/Assets/gltf/table_small_decorated_B.gltf", "start": 42819560, "end": 42822638}, {"filename": "/assets/dungeons/Assets/gltf/torch.bin", "start": 42822638, "end": 42833150}, {"filename": "/assets/dungeons/Assets/gltf/torch.gltf", "start": 42833150, "end": 42836182}, {"filename": "/assets/dungeons/Assets/gltf/torch_lit.bin", "start": 42836182, "end": 42847946}, {"filename": "/assets/dungeons/Assets/gltf/torch_lit.gltf", "start": 42847946, "end": 42850991}, {"filename": "/assets/dungeons/Assets/gltf/torch_mounted.bin", "start": 42850991, "end": 42865171}, {"filename": "/assets/dungeons/Assets/gltf/torch_mounted.gltf", "start": 42865171, "end": 42868231}, {"filename": "/assets/dungeons/Assets/gltf/trunk_large_A.bin", "start": 42868231, "end": 42880911}, {"filename": "/assets/dungeons/Assets/gltf/trunk_large_A.gltf", "start": 42880911, "end": 42883972}, {"filename": "/assets/dungeons/Assets/gltf/trunk_large_B.bin", "start": 42883972, "end": 42896652}, {"filename": "/assets/dungeons/Assets/gltf/trunk_large_B.gltf", "start": 42896652, "end": 42899713}, {"filename": "/assets/dungeons/Assets/gltf/trunk_large_C.bin", "start": 42899713, "end": 42912393}, {"filename": "/assets/dungeons/Assets/gltf/trunk_large_C.gltf", "start": 42912393, "end": 42915454}, {"filename": "/assets/dungeons/Assets/gltf/trunk_medium_A.bin", "start": 42915454, "end": 42924470}, {"filename": "/assets/dungeons/Assets/gltf/trunk_medium_A.gltf", "start": 42924470, "end": 42927533}, {"filename": "/assets/dungeons/Assets/gltf/trunk_medium_B.bin", "start": 42927533, "end": 42936549}, {"filename": "/assets/dungeons/Assets/gltf/trunk_medium_B.gltf", "start": 42936549, "end": 42939612}, {"filename": "/assets/dungeons/Assets/gltf/trunk_medium_C.bin", "start": 42939612, "end": 42948628}, {"filename": "/assets/dungeons/Assets/gltf/trunk_medium_C.gltf", "start": 42948628, "end": 42951691}, {"filename": "/assets/dungeons/Assets/gltf/trunk_small_A.bin", "start": 42951691, "end": 42964371}, {"filename": "/assets/dungeons/Assets/gltf/trunk_small_A.gltf", "start": 42964371, "end": 42967434}, {"filename": "/assets/dungeons/Assets/gltf/trunk_small_B.bin", "start": 42967434, "end": 42980114}, {"filename": "/assets/dungeons/Assets/gltf/trunk_small_B.gltf", "start": 42980114, "end": 42983177}, {"filename": "/assets/dungeons/Assets/gltf/trunk_small_C.bin", "start": 42983177, "end": 42995857}, {"filename": "/assets/dungeons/Assets/gltf/trunk_small_C.gltf", "start": 42995857, "end": 42998920}, {"filename": "/assets/dungeons/Assets/gltf/wall.bin", "start": 42998920, "end": 43034204}, {"filename": "/assets/dungeons/Assets/gltf/wall.gltf", "start": 43034204, "end": 43037144}, {"filename": "/assets/dungeons/Assets/gltf/wall_Tsplit.bin", "start": 43037144, "end": 43078704}, {"filename": "/assets/dungeons/Assets/gltf/wall_Tsplit.gltf", "start": 43078704, "end": 43081693}, {"filename": "/assets/dungeons/Assets/gltf/wall_Tsplit_sloped.bin", "start": 43081693, "end": 43120705}, {"filename": "/assets/dungeons/Assets/gltf/wall_Tsplit_sloped.gltf", "start": 43120705, "end": 43123717}, {"filename": "/assets/dungeons/Assets/gltf/wall_arched.bin", "start": 43123717, "end": 43174429}, {"filename": "/assets/dungeons/Assets/gltf/wall_arched.gltf", "start": 43174429, "end": 43177437}, {"filename": "/assets/dungeons/Assets/gltf/wall_archedwindow_gated.bin", "start": 43177437, "end": 43224325}, {"filename": "/assets/dungeons/Assets/gltf/wall_archedwindow_gated.gltf", "start": 43224325, "end": 43227353}, {"filename": "/assets/dungeons/Assets/gltf/wall_archedwindow_gated_scaffold.bin", "start": 43227353, "end": 43270501}, {"filename": "/assets/dungeons/Assets/gltf/wall_archedwindow_gated_scaffold.gltf", "start": 43270501, "end": 43273555}, {"filename": "/assets/dungeons/Assets/gltf/wall_archedwindow_open.bin", "start": 43273555, "end": 43313687}, {"filename": "/assets/dungeons/Assets/gltf/wall_archedwindow_open.gltf", "start": 43313687, "end": 43316681}, {"filename": "/assets/dungeons/Assets/gltf/wall_broken.bin", "start": 43316681, "end": 43368809}, {"filename": "/assets/dungeons/Assets/gltf/wall_broken.gltf", "start": 43368809, "end": 43371801}, {"filename": "/assets/dungeons/Assets/gltf/wall_corner.bin", "start": 43371801, "end": 43402973}, {"filename": "/assets/dungeons/Assets/gltf/wall_corner.gltf", "start": 43402973, "end": 43405961}, {"filename": "/assets/dungeons/Assets/gltf/wall_corner_gated.bin", "start": 43405961, "end": 43438649}, {"filename": "/assets/dungeons/Assets/gltf/wall_corner_gated.gltf", "start": 43438649, "end": 43441670}, {"filename": "/assets/dungeons/Assets/gltf/wall_corner_scaffold.bin", "start": 43441670, "end": 43477882}, {"filename": "/assets/dungeons/Assets/gltf/wall_corner_scaffold.gltf", "start": 43477882, "end": 43480900}, {"filename": "/assets/dungeons/Assets/gltf/wall_corner_small.bin", "start": 43480900, "end": 43497216}, {"filename": "/assets/dungeons/Assets/gltf/wall_corner_small.gltf", "start": 43497216, "end": 43500236}, {"filename": "/assets/dungeons/Assets/gltf/wall_cracked.bin", "start": 43500236, "end": 43603496}, {"filename": "/assets/dungeons/Assets/gltf/wall_cracked.gltf", "start": 43603496, "end": 43606493}, {"filename": "/assets/dungeons/Assets/gltf/wall_crossing.bin", "start": 43606493, "end": 43654329}, {"filename": "/assets/dungeons/Assets/gltf/wall_crossing.gltf", "start": 43654329, "end": 43657308}, {"filename": "/assets/dungeons/Assets/gltf/wall_doorway.bin", "start": 43657308, "end": 43709668}, {"filename": "/assets/dungeons/Assets/gltf/wall_doorway.gltf", "start": 43709668, "end": 43714713}, {"filename": "/assets/dungeons/Assets/gltf/wall_doorway_Tsplit.bin", "start": 43714713, "end": 43788201}, {"filename": "/assets/dungeons/Assets/gltf/wall_doorway_Tsplit.gltf", "start": 43788201, "end": 43791200}, {"filename": "/assets/dungeons/Assets/gltf/wall_doorway_scaffold.bin", "start": 43791200, "end": 43849876}, {"filename": "/assets/dungeons/Assets/gltf/wall_doorway_scaffold.gltf", "start": 43849876, "end": 43854968}, {"filename": "/assets/dungeons/Assets/gltf/wall_doorway_sides.bin", "start": 43854968, "end": 43933448}, {"filename": "/assets/dungeons/Assets/gltf/wall_doorway_sides.gltf", "start": 43933448, "end": 43936460}, {"filename": "/assets/dungeons/Assets/gltf/wall_endcap.bin", "start": 43936460, "end": 43952896}, {"filename": "/assets/dungeons/Assets/gltf/wall_endcap.gltf", "start": 43952896, "end": 43955881}, {"filename": "/assets/dungeons/Assets/gltf/wall_gated.bin", "start": 43955881, "end": 43984977}, {"filename": "/assets/dungeons/Assets/gltf/wall_gated.gltf", "start": 43984977, "end": 43987959}, {"filename": "/assets/dungeons/Assets/gltf/wall_half.bin", "start": 43987959, "end": 44007319}, {"filename": "/assets/dungeons/Assets/gltf/wall_half.gltf", "start": 44007319, "end": 44010319}, {"filename": "/assets/dungeons/Assets/gltf/wall_half_endcap.bin", "start": 44010319, "end": 44029559}, {"filename": "/assets/dungeons/Assets/gltf/wall_half_endcap.gltf", "start": 44029559, "end": 44032578}, {"filename": "/assets/dungeons/Assets/gltf/wall_half_endcap_sloped.bin", "start": 44032578, "end": 44048678}, {"filename": "/assets/dungeons/Assets/gltf/wall_half_endcap_sloped.gltf", "start": 44048678, "end": 44051717}, {"filename": "/assets/dungeons/Assets/gltf/wall_open_scaffold.bin", "start": 44051717, "end": 44064285}, {"filename": "/assets/dungeons/Assets/gltf/wall_open_scaffold.gltf", "start": 44064285, "end": 44067289}, {"filename": "/assets/dungeons/Assets/gltf/wall_pillar.bin", "start": 44067289, "end": 44104289}, {"filename": "/assets/dungeons/Assets/gltf/wall_pillar.gltf", "start": 44104289, "end": 44107266}, {"filename": "/assets/dungeons/Assets/gltf/wall_scaffold.bin", "start": 44107266, "end": 44148530}, {"filename": "/assets/dungeons/Assets/gltf/wall_scaffold.gltf", "start": 44148530, "end": 44151527}, {"filename": "/assets/dungeons/Assets/gltf/wall_shelves.bin", "start": 44151527, "end": 44253307}, {"filename": "/assets/dungeons/Assets/gltf/wall_shelves.gltf", "start": 44253307, "end": 44256288}, {"filename": "/assets/dungeons/Assets/gltf/wall_sloped.bin", "start": 44256288, "end": 44274888}, {"filename": "/assets/dungeons/Assets/gltf/wall_sloped.gltf", "start": 44274888, "end": 44277872}, {"filename": "/assets/dungeons/Assets/gltf/wall_window_closed.bin", "start": 44277872, "end": 44321792}, {"filename": "/assets/dungeons/Assets/gltf/wall_window_closed.gltf", "start": 44321792, "end": 44324804}, {"filename": "/assets/dungeons/Assets/gltf/wall_window_closed_scaffold.bin", "start": 44324804, "end": 44373764}, {"filename": "/assets/dungeons/Assets/gltf/wall_window_closed_scaffold.gltf", "start": 44373764, "end": 44376804}, {"filename": "/assets/dungeons/Assets/gltf/wall_window_open.bin", "start": 44376804, "end": 44416524}, {"filename": "/assets/dungeons/Assets/gltf/wall_window_open.gltf", "start": 44416524, "end": 44419530}, {"filename": "/assets/dungeons/Assets/gltf/wall_window_open_scaffold.bin", "start": 44419530, "end": 44464290}, {"filename": "/assets/dungeons/Assets/gltf/wall_window_open_scaffold.gltf", "start": 44464290, "end": 44467324}, {"filename": "/assets/dungeons/Assets/obj/banner_blue.mtl", "start": 44467324, "end": 44467610}, {"filename": "/assets/dungeons/Assets/obj/banner_blue.obj", "start": 44467610, "end": 44476379}, {"filename": "/assets/dungeons/Assets/obj/banner_brown.mtl", "start": 44476379, "end": 44476665}, {"filename": "/assets/dungeons/Assets/obj/banner_brown.obj", "start": 44476665, "end": 44485436}, {"filename": "/assets/dungeons/Assets/obj/banner_green.mtl", "start": 44485436, "end": 44485722}, {"filename": "/assets/dungeons/Assets/obj/banner_green.obj", "start": 44485722, "end": 44494493}, {"filename": "/assets/dungeons/Assets/obj/banner_patternA_blue.mtl", "start": 44494493, "end": 44494779}, {"filename": "/assets/dungeons/Assets/obj/banner_patternA_blue.obj", "start": 44494779, "end": 44504775}, {"filename": "/assets/dungeons/Assets/obj/banner_patternA_brown.mtl", "start": 44504775, "end": 44505061}, {"filename": "/assets/dungeons/Assets/obj/banner_patternA_brown.obj", "start": 44505061, "end": 44515059}, {"filename": "/assets/dungeons/Assets/obj/banner_patternA_green.mtl", "start": 44515059, "end": 44515345}, {"filename": "/assets/dungeons/Assets/obj/banner_patternA_green.obj", "start": 44515345, "end": 44525343}, {"filename": "/assets/dungeons/Assets/obj/banner_patternA_red.mtl", "start": 44525343, "end": 44525629}, {"filename": "/assets/dungeons/Assets/obj/banner_patternA_red.obj", "start": 44525629, "end": 44535623}, {"filename": "/assets/dungeons/Assets/obj/banner_patternA_white.mtl", "start": 44535623, "end": 44535909}, {"filename": "/assets/dungeons/Assets/obj/banner_patternA_white.obj", "start": 44535909, "end": 44545907}, {"filename": "/assets/dungeons/Assets/obj/banner_patternA_yellow.mtl", "start": 44545907, "end": 44546193}, {"filename": "/assets/dungeons/Assets/obj/banner_patternA_yellow.obj", "start": 44546193, "end": 44556193}, {"filename": "/assets/dungeons/Assets/obj/banner_patternB_blue.mtl", "start": 44556193, "end": 44556479}, {"filename": "/assets/dungeons/Assets/obj/banner_patternB_blue.obj", "start": 44556479, "end": 44565486}, {"filename": "/assets/dungeons/Assets/obj/banner_patternB_brown.mtl", "start": 44565486, "end": 44565772}, {"filename": "/assets/dungeons/Assets/obj/banner_patternB_brown.obj", "start": 44565772, "end": 44574781}, {"filename": "/assets/dungeons/Assets/obj/banner_patternB_green.mtl", "start": 44574781, "end": 44575067}, {"filename": "/assets/dungeons/Assets/obj/banner_patternB_green.obj", "start": 44575067, "end": 44584076}, {"filename": "/assets/dungeons/Assets/obj/banner_patternB_red.mtl", "start": 44584076, "end": 44584362}, {"filename": "/assets/dungeons/Assets/obj/banner_patternB_red.obj", "start": 44584362, "end": 44593367}, {"filename": "/assets/dungeons/Assets/obj/banner_patternB_white.mtl", "start": 44593367, "end": 44593653}, {"filename": "/assets/dungeons/Assets/obj/banner_patternB_white.obj", "start": 44593653, "end": 44602662}, {"filename": "/assets/dungeons/Assets/obj/banner_patternB_yellow.mtl", "start": 44602662, "end": 44602948}, {"filename": "/assets/dungeons/Assets/obj/banner_patternB_yellow.obj", "start": 44602948, "end": 44611959}, {"filename": "/assets/dungeons/Assets/obj/banner_patternC_blue.mtl", "start": 44611959, "end": 44612245}, {"filename": "/assets/dungeons/Assets/obj/banner_patternC_blue.obj", "start": 44612245, "end": 44621252}, {"filename": "/assets/dungeons/Assets/obj/banner_patternC_brown.mtl", "start": 44621252, "end": 44621538}, {"filename": "/assets/dungeons/Assets/obj/banner_patternC_brown.obj", "start": 44621538, "end": 44630547}, {"filename": "/assets/dungeons/Assets/obj/banner_patternC_green.mtl", "start": 44630547, "end": 44630833}, {"filename": "/assets/dungeons/Assets/obj/banner_patternC_green.obj", "start": 44630833, "end": 44639842}, {"filename": "/assets/dungeons/Assets/obj/banner_patternC_red.mtl", "start": 44639842, "end": 44640128}, {"filename": "/assets/dungeons/Assets/obj/banner_patternC_red.obj", "start": 44640128, "end": 44649133}, {"filename": "/assets/dungeons/Assets/obj/banner_patternC_white.mtl", "start": 44649133, "end": 44649419}, {"filename": "/assets/dungeons/Assets/obj/banner_patternC_white.obj", "start": 44649419, "end": 44658428}, {"filename": "/assets/dungeons/Assets/obj/banner_patternC_yellow.mtl", "start": 44658428, "end": 44658714}, {"filename": "/assets/dungeons/Assets/obj/banner_patternC_yellow.obj", "start": 44658714, "end": 44667725}, {"filename": "/assets/dungeons/Assets/obj/banner_red.mtl", "start": 44667725, "end": 44668011}, {"filename": "/assets/dungeons/Assets/obj/banner_red.obj", "start": 44668011, "end": 44676778}, {"filename": "/assets/dungeons/Assets/obj/banner_shield_blue.mtl", "start": 44676778, "end": 44677064}, {"filename": "/assets/dungeons/Assets/obj/banner_shield_blue.obj", "start": 44677064, "end": 44737287}, {"filename": "/assets/dungeons/Assets/obj/banner_shield_brown.mtl", "start": 44737287, "end": 44737573}, {"filename": "/assets/dungeons/Assets/obj/banner_shield_brown.obj", "start": 44737573, "end": 44797798}, {"filename": "/assets/dungeons/Assets/obj/banner_shield_green.mtl", "start": 44797798, "end": 44798084}, {"filename": "/assets/dungeons/Assets/obj/banner_shield_green.obj", "start": 44798084, "end": 44858309}, {"filename": "/assets/dungeons/Assets/obj/banner_shield_red.mtl", "start": 44858309, "end": 44858595}, {"filename": "/assets/dungeons/Assets/obj/banner_shield_red.obj", "start": 44858595, "end": 44918816}, {"filename": "/assets/dungeons/Assets/obj/banner_shield_white.mtl", "start": 44918816, "end": 44919102}, {"filename": "/assets/dungeons/Assets/obj/banner_shield_white.obj", "start": 44919102, "end": 44979327}, {"filename": "/assets/dungeons/Assets/obj/banner_shield_yellow.mtl", "start": 44979327, "end": 44979613}, {"filename": "/assets/dungeons/Assets/obj/banner_shield_yellow.obj", "start": 44979613, "end": 45039840}, {"filename": "/assets/dungeons/Assets/obj/banner_thin_blue.mtl", "start": 45039840, "end": 45040126}, {"filename": "/assets/dungeons/Assets/obj/banner_thin_blue.obj", "start": 45040126, "end": 45047388}, {"filename": "/assets/dungeons/Assets/obj/banner_thin_brown.mtl", "start": 45047388, "end": 45047674}, {"filename": "/assets/dungeons/Assets/obj/banner_thin_brown.obj", "start": 45047674, "end": 45054938}, {"filename": "/assets/dungeons/Assets/obj/banner_thin_green.mtl", "start": 45054938, "end": 45055224}, {"filename": "/assets/dungeons/Assets/obj/banner_thin_green.obj", "start": 45055224, "end": 45062488}, {"filename": "/assets/dungeons/Assets/obj/banner_thin_red.mtl", "start": 45062488, "end": 45062774}, {"filename": "/assets/dungeons/Assets/obj/banner_thin_red.obj", "start": 45062774, "end": 45070034}, {"filename": "/assets/dungeons/Assets/obj/banner_thin_white.mtl", "start": 45070034, "end": 45070320}, {"filename": "/assets/dungeons/Assets/obj/banner_thin_white.obj", "start": 45070320, "end": 45077584}, {"filename": "/assets/dungeons/Assets/obj/banner_thin_yellow.mtl", "start": 45077584, "end": 45077870}, {"filename": "/assets/dungeons/Assets/obj/banner_thin_yellow.obj", "start": 45077870, "end": 45085136}, {"filename": "/assets/dungeons/Assets/obj/banner_triple_blue.mtl", "start": 45085136, "end": 45085422}, {"filename": "/assets/dungeons/Assets/obj/banner_triple_blue.obj", "start": 45085422, "end": 45108000}, {"filename": "/assets/dungeons/Assets/obj/banner_triple_brown.mtl", "start": 45108000, "end": 45108286}, {"filename": "/assets/dungeons/Assets/obj/banner_triple_brown.obj", "start": 45108286, "end": 45130866}, {"filename": "/assets/dungeons/Assets/obj/banner_triple_green.mtl", "start": 45130866, "end": 45131152}, {"filename": "/assets/dungeons/Assets/obj/banner_triple_green.obj", "start": 45131152, "end": 45153732}, {"filename": "/assets/dungeons/Assets/obj/banner_triple_red.mtl", "start": 45153732, "end": 45154018}, {"filename": "/assets/dungeons/Assets/obj/banner_triple_red.obj", "start": 45154018, "end": 45176594}, {"filename": "/assets/dungeons/Assets/obj/banner_triple_white.mtl", "start": 45176594, "end": 45176880}, {"filename": "/assets/dungeons/Assets/obj/banner_triple_white.obj", "start": 45176880, "end": 45199460}, {"filename": "/assets/dungeons/Assets/obj/banner_triple_yellow.mtl", "start": 45199460, "end": 45199746}, {"filename": "/assets/dungeons/Assets/obj/banner_triple_yellow.obj", "start": 45199746, "end": 45222328}, {"filename": "/assets/dungeons/Assets/obj/banner_white.mtl", "start": 45222328, "end": 45222614}, {"filename": "/assets/dungeons/Assets/obj/banner_white.obj", "start": 45222614, "end": 45231385}, {"filename": "/assets/dungeons/Assets/obj/banner_yellow.mtl", "start": 45231385, "end": 45231671}, {"filename": "/assets/dungeons/Assets/obj/banner_yellow.obj", "start": 45231671, "end": 45240444}, {"filename": "/assets/dungeons/Assets/obj/barrel_large.mtl", "start": 45240444, "end": 45240730}, {"filename": "/assets/dungeons/Assets/obj/barrel_large.obj", "start": 45240730, "end": 45286170}, {"filename": "/assets/dungeons/Assets/obj/barrel_large_decorated.mtl", "start": 45286170, "end": 45286456}, {"filename": "/assets/dungeons/Assets/obj/barrel_large_decorated.obj", "start": 45286456, "end": 45376822}, {"filename": "/assets/dungeons/Assets/obj/barrel_small.mtl", "start": 45376822, "end": 45377108}, {"filename": "/assets/dungeons/Assets/obj/barrel_small.obj", "start": 45377108, "end": 45393778}, {"filename": "/assets/dungeons/Assets/obj/barrel_small_stack.mtl", "start": 45393778, "end": 45394064}, {"filename": "/assets/dungeons/Assets/obj/barrel_small_stack.obj", "start": 45394064, "end": 45483563}, {"filename": "/assets/dungeons/Assets/obj/barrier.mtl", "start": 45483563, "end": 45483849}, {"filename": "/assets/dungeons/Assets/obj/barrier.obj", "start": 45483849, "end": 45491318}, {"filename": "/assets/dungeons/Assets/obj/barrier_colum_half.mtl", "start": 45491318, "end": 45491604}, {"filename": "/assets/dungeons/Assets/obj/barrier_colum_half.obj", "start": 45491604, "end": 45498756}, {"filename": "/assets/dungeons/Assets/obj/barrier_column.mtl", "start": 45498756, "end": 45499042}, {"filename": "/assets/dungeons/Assets/obj/barrier_column.obj", "start": 45499042, "end": 45508021}, {"filename": "/assets/dungeons/Assets/obj/barrier_corner.mtl", "start": 45508021, "end": 45508307}, {"filename": "/assets/dungeons/Assets/obj/barrier_corner.obj", "start": 45508307, "end": 45519623}, {"filename": "/assets/dungeons/Assets/obj/barrier_half.mtl", "start": 45519623, "end": 45519909}, {"filename": "/assets/dungeons/Assets/obj/barrier_half.obj", "start": 45519909, "end": 45523830}, {"filename": "/assets/dungeons/Assets/obj/bed_decorated.mtl", "start": 45523830, "end": 45524116}, {"filename": "/assets/dungeons/Assets/obj/bed_decorated.obj", "start": 45524116, "end": 45642974}, {"filename": "/assets/dungeons/Assets/obj/bed_floor.mtl", "start": 45642974, "end": 45643260}, {"filename": "/assets/dungeons/Assets/obj/bed_floor.obj", "start": 45643260, "end": 45659763}, {"filename": "/assets/dungeons/Assets/obj/bed_frame.mtl", "start": 45659763, "end": 45660049}, {"filename": "/assets/dungeons/Assets/obj/bed_frame.obj", "start": 45660049, "end": 45683059}, {"filename": "/assets/dungeons/Assets/obj/bottle_A_brown.mtl", "start": 45683059, "end": 45683345}, {"filename": "/assets/dungeons/Assets/obj/bottle_A_brown.obj", "start": 45683345, "end": 45695982}, {"filename": "/assets/dungeons/Assets/obj/bottle_A_green.mtl", "start": 45695982, "end": 45696268}, {"filename": "/assets/dungeons/Assets/obj/bottle_A_green.obj", "start": 45696268, "end": 45708905}, {"filename": "/assets/dungeons/Assets/obj/bottle_A_labeled_brown.mtl", "start": 45708905, "end": 45709191}, {"filename": "/assets/dungeons/Assets/obj/bottle_A_labeled_brown.obj", "start": 45709191, "end": 45721932}, {"filename": "/assets/dungeons/Assets/obj/bottle_A_labeled_green.mtl", "start": 45721932, "end": 45722218}, {"filename": "/assets/dungeons/Assets/obj/bottle_A_labeled_green.obj", "start": 45722218, "end": 45734959}, {"filename": "/assets/dungeons/Assets/obj/bottle_B_brown.mtl", "start": 45734959, "end": 45735245}, {"filename": "/assets/dungeons/Assets/obj/bottle_B_brown.obj", "start": 45735245, "end": 45747882}, {"filename": "/assets/dungeons/Assets/obj/bottle_B_green.mtl", "start": 45747882, "end": 45748168}, {"filename": "/assets/dungeons/Assets/obj/bottle_B_green.obj", "start": 45748168, "end": 45760805}, {"filename": "/assets/dungeons/Assets/obj/bottle_C_brown.mtl", "start": 45760805, "end": 45761091}, {"filename": "/assets/dungeons/Assets/obj/bottle_C_brown.obj", "start": 45761091, "end": 45773729}, {"filename": "/assets/dungeons/Assets/obj/bottle_C_green.mtl", "start": 45773729, "end": 45774015}, {"filename": "/assets/dungeons/Assets/obj/bottle_C_green.obj", "start": 45774015, "end": 45786653}, {"filename": "/assets/dungeons/Assets/obj/box_large.mtl", "start": 45786653, "end": 45786939}, {"filename": "/assets/dungeons/Assets/obj/box_large.obj", "start": 45786939, "end": 45801709}, {"filename": "/assets/dungeons/Assets/obj/box_small.mtl", "start": 45801709, "end": 45801995}, {"filename": "/assets/dungeons/Assets/obj/box_small.obj", "start": 45801995, "end": 45816764}, {"filename": "/assets/dungeons/Assets/obj/box_small_decorated.mtl", "start": 45816764, "end": 45817050}, {"filename": "/assets/dungeons/Assets/obj/box_small_decorated.obj", "start": 45817050, "end": 45891650}, {"filename": "/assets/dungeons/Assets/obj/box_stacked.mtl", "start": 45891650, "end": 45891936}, {"filename": "/assets/dungeons/Assets/obj/box_stacked.obj", "start": 45891936, "end": 46083719}, {"filename": "/assets/dungeons/Assets/obj/candle.mtl", "start": 46083719, "end": 46084005}, {"filename": "/assets/dungeons/Assets/obj/candle.obj", "start": 46084005, "end": 46089527}, {"filename": "/assets/dungeons/Assets/obj/candle_lit.mtl", "start": 46089527, "end": 46089813}, {"filename": "/assets/dungeons/Assets/obj/candle_lit.obj", "start": 46089813, "end": 46098733}, {"filename": "/assets/dungeons/Assets/obj/candle_melted.mtl", "start": 46098733, "end": 46099019}, {"filename": "/assets/dungeons/Assets/obj/candle_melted.obj", "start": 46099019, "end": 46104624}, {"filename": "/assets/dungeons/Assets/obj/candle_thin.mtl", "start": 46104624, "end": 46104910}, {"filename": "/assets/dungeons/Assets/obj/candle_thin.obj", "start": 46104910, "end": 46110442}, {"filename": "/assets/dungeons/Assets/obj/candle_thin_lit.mtl", "start": 46110442, "end": 46110728}, {"filename": "/assets/dungeons/Assets/obj/candle_thin_lit.obj", "start": 46110728, "end": 46119658}, {"filename": "/assets/dungeons/Assets/obj/candle_triple.mtl", "start": 46119658, "end": 46119944}, {"filename": "/assets/dungeons/Assets/obj/candle_triple.obj", "start": 46119944, "end": 46136831}, {"filename": "/assets/dungeons/Assets/obj/ceiling_tile.mtl", "start": 46136831, "end": 46137117}, {"filename": "/assets/dungeons/Assets/obj/ceiling_tile.obj", "start": 46137117, "end": 46140775}, {"filename": "/assets/dungeons/Assets/obj/chair.mtl", "start": 46140775, "end": 46141061}, {"filename": "/assets/dungeons/Assets/obj/chair.obj", "start": 46141061, "end": 46166429}, {"filename": "/assets/dungeons/Assets/obj/chest.mtl", "start": 46166429, "end": 46166715}, {"filename": "/assets/dungeons/Assets/obj/chest.obj", "start": 46166715, "end": 46186085}, {"filename": "/assets/dungeons/Assets/obj/chest_gold.mtl", "start": 46186085, "end": 46186371}, {"filename": "/assets/dungeons/Assets/obj/chest_gold.obj", "start": 46186371, "end": 46270082}, {"filename": "/assets/dungeons/Assets/obj/chest_gold_lid.mtl", "start": 46270082, "end": 46270368}, {"filename": "/assets/dungeons/Assets/obj/chest_gold_lid.obj", "start": 46270368, "end": 46308785}, {"filename": "/assets/dungeons/Assets/obj/chest_lid.mtl", "start": 46308785, "end": 46309071}, {"filename": "/assets/dungeons/Assets/obj/chest_lid.obj", "start": 46309071, "end": 46347478}, {"filename": "/assets/dungeons/Assets/obj/coin.mtl", "start": 46347478, "end": 46347764}, {"filename": "/assets/dungeons/Assets/obj/coin.obj", "start": 46347764, "end": 46354447}, {"filename": "/assets/dungeons/Assets/obj/coin_stack_large.mtl", "start": 46354447, "end": 46354733}, {"filename": "/assets/dungeons/Assets/obj/coin_stack_large.obj", "start": 46354733, "end": 46486720}, {"filename": "/assets/dungeons/Assets/obj/coin_stack_medium.mtl", "start": 46486720, "end": 46487006}, {"filename": "/assets/dungeons/Assets/obj/coin_stack_medium.obj", "start": 46487006, "end": 46560472}, {"filename": "/assets/dungeons/Assets/obj/coin_stack_small.mtl", "start": 46560472, "end": 46560758}, {"filename": "/assets/dungeons/Assets/obj/coin_stack_small.obj", "start": 46560758, "end": 46592730}, {"filename": "/assets/dungeons/Assets/obj/column.mtl", "start": 46592730, "end": 46593016}, {"filename": "/assets/dungeons/Assets/obj/column.obj", "start": 46593016, "end": 46596581}, {"filename": "/assets/dungeons/Assets/obj/crates_stacked.mtl", "start": 46596581, "end": 46596867}, {"filename": "/assets/dungeons/Assets/obj/crates_stacked.obj", "start": 46596867, "end": 46716364}, {"filename": "/assets/dungeons/Assets/obj/dungeon_texture.png", "start": 46716364, "end": 46733411}, {"filename": "/assets/dungeons/Assets/obj/floor_dirt_large.mtl", "start": 46733411, "end": 46733697}, {"filename": "/assets/dungeons/Assets/obj/floor_dirt_large.obj", "start": 46733697, "end": 46745405}, {"filename": "/assets/dungeons/Assets/obj/floor_dirt_large_rocky.mtl", "start": 46745405, "end": 46745691}, {"filename": "/assets/dungeons/Assets/obj/floor_dirt_large_rocky.obj", "start": 46745691, "end": 46767693}, {"filename": "/assets/dungeons/Assets/obj/floor_dirt_small_A.mtl", "start": 46767693, "end": 46767979}, {"filename": "/assets/dungeons/Assets/obj/floor_dirt_small_A.obj", "start": 46767979, "end": 46772985}, {"filename": "/assets/dungeons/Assets/obj/floor_dirt_small_B.mtl", "start": 46772985, "end": 46773271}, {"filename": "/assets/dungeons/Assets/obj/floor_dirt_small_B.obj", "start": 46773271, "end": 46776991}, {"filename": "/assets/dungeons/Assets/obj/floor_dirt_small_C.mtl", "start": 46776991, "end": 46777277}, {"filename": "/assets/dungeons/Assets/obj/floor_dirt_small_C.obj", "start": 46777277, "end": 46780847}, {"filename": "/assets/dungeons/Assets/obj/floor_dirt_small_D.mtl", "start": 46780847, "end": 46781133}, {"filename": "/assets/dungeons/Assets/obj/floor_dirt_small_D.obj", "start": 46781133, "end": 46783122}, {"filename": "/assets/dungeons/Assets/obj/floor_dirt_small_corner.mtl", "start": 46783122, "end": 46783408}, {"filename": "/assets/dungeons/Assets/obj/floor_dirt_small_corner.obj", "start": 46783408, "end": 46788895}, {"filename": "/assets/dungeons/Assets/obj/floor_dirt_small_weeds.mtl", "start": 46788895, "end": 46789181}, {"filename": "/assets/dungeons/Assets/obj/floor_dirt_small_weeds.obj", "start": 46789181, "end": 46827105}, {"filename": "/assets/dungeons/Assets/obj/floor_foundation_allsides.mtl", "start": 46827105, "end": 46827391}, {"filename": "/assets/dungeons/Assets/obj/floor_foundation_allsides.obj", "start": 46827391, "end": 46862102}, {"filename": "/assets/dungeons/Assets/obj/floor_foundation_corner.mtl", "start": 46862102, "end": 46862388}, {"filename": "/assets/dungeons/Assets/obj/floor_foundation_corner.obj", "start": 46862388, "end": 46880945}, {"filename": "/assets/dungeons/Assets/obj/floor_foundation_diagonal_corner.mtl", "start": 46880945, "end": 46881231}, {"filename": "/assets/dungeons/Assets/obj/floor_foundation_diagonal_corner.obj", "start": 46881231, "end": 46891020}, {"filename": "/assets/dungeons/Assets/obj/floor_foundation_front.mtl", "start": 46891020, "end": 46891306}, {"filename": "/assets/dungeons/Assets/obj/floor_foundation_front.obj", "start": 46891306, "end": 46901666}, {"filename": "/assets/dungeons/Assets/obj/floor_foundation_front_and_back.mtl", "start": 46901666, "end": 46901952}, {"filename": "/assets/dungeons/Assets/obj/floor_foundation_front_and_back.obj", "start": 46901952, "end": 46920547}, {"filename": "/assets/dungeons/Assets/obj/floor_foundation_front_and_sides.mtl", "start": 46920547, "end": 46920833}, {"filename": "/assets/dungeons/Assets/obj/floor_foundation_front_and_sides.obj", "start": 46920833, "end": 46947487}, {"filename": "/assets/dungeons/Assets/obj/floor_tile_big_grate.mtl", "start": 46947487, "end": 46947773}, {"filename": "/assets/dungeons/Assets/obj/floor_tile_big_grate.obj", "start": 46947773, "end": 46973610}, {"filename": "/assets/dungeons/Assets/obj/floor_tile_big_grate_open.mtl", "start": 46973610, "end": 46973896}, {"filename": "/assets/dungeons/Assets/obj/floor_tile_big_grate_open.obj", "start": 46973896, "end": 46999142}, {"filename": "/assets/dungeons/Assets/obj/floor_tile_big_spikes.mtl", "start": 46999142, "end": 46999428}, {"filename": "/assets/dungeons/Assets/obj/floor_tile_big_spikes.obj", "start": 46999428, "end": 47038621}, {"filename": "/assets/dungeons/Assets/obj/floor_tile_big_spikes_spikes.mtl", "start": 47038621, "end": 47038907}, {"filename": "/assets/dungeons/Assets/obj/floor_tile_big_spikes_spikes.obj", "start": 47038907, "end": 47047880}, {"filename": "/assets/dungeons/Assets/obj/floor_tile_extralarge_grates.mtl", "start": 47047880, "end": 47048166}, {"filename": "/assets/dungeons/Assets/obj/floor_tile_extralarge_grates.obj", "start": 47048166, "end": 47114306}, {"filename": "/assets/dungeons/Assets/obj/floor_tile_extralarge_grates_open.mtl", "start": 47114306, "end": 47114592}, {"filename": "/assets/dungeons/Assets/obj/floor_tile_extralarge_grates_open.obj", "start": 47114592, "end": 47180222}, {"filename": "/assets/dungeons/Assets/obj/floor_tile_grate.mtl", "start": 47180222, "end": 47180508}, {"filename": "/assets/dungeons/Assets/obj/floor_tile_grate.obj", "start": 47180508, "end": 47193698}, {"filename": "/assets/dungeons/Assets/obj/floor_tile_grate_open.mtl", "start": 47193698, "end": 47193984}, {"filename": "/assets/dungeons/Assets/obj/floor_tile_grate_open.obj", "start": 47193984, "end": 47207062}, {"filename": "/assets/dungeons/Assets/obj/floor_tile_large.mtl", "start": 47207062, "end": 47207348}, {"filename": "/assets/dungeons/Assets/obj/floor_tile_large.obj", "start": 47207348, "end": 47220027}, {"filename": "/assets/dungeons/Assets/obj/floor_tile_large_rocks.mtl", "start": 47220027, "end": 47220313}, {"filename": "/assets/dungeons/Assets/obj/floor_tile_large_rocks.obj", "start": 47220313, "end": 47253207}, {"filename": "/assets/dungeons/Assets/obj/floor_tile_small.mtl", "start": 47253207, "end": 47253493}, {"filename": "/assets/dungeons/Assets/obj/floor_tile_small.obj", "start": 47253493, "end": 47258483}, {"filename": "/assets/dungeons/Assets/obj/floor_tile_small_broken_A.mtl", "start": 47258483, "end": 47258769}, {"filename": "/assets/dungeons/Assets/obj/floor_tile_small_broken_A.obj", "start": 47258769, "end": 47265965}, {"filename": "/assets/dungeons/Assets/obj/floor_tile_small_broken_B.mtl", "start": 47265965, "end": 47266251}, {"filename": "/assets/dungeons/Assets/obj/floor_tile_small_broken_B.obj", "start": 47266251, "end": 47272108}, {"filename": "/assets/dungeons/Assets/obj/floor_tile_small_corner.mtl", "start": 47272108, "end": 47272394}, {"filename": "/assets/dungeons/Assets/obj/floor_tile_small_corner.obj", "start": 47272394, "end": 47274996}, {"filename": "/assets/dungeons/Assets/obj/floor_tile_small_decorated.mtl", "start": 47274996, "end": 47275282}, {"filename": "/assets/dungeons/Assets/obj/floor_tile_small_decorated.obj", "start": 47275282, "end": 47311992}, {"filename": "/assets/dungeons/Assets/obj/floor_tile_small_weeds_A.mtl", "start": 47311992, "end": 47312278}, {"filename": "/assets/dungeons/Assets/obj/floor_tile_small_weeds_A.obj", "start": 47312278, "end": 47363694}, {"filename": "/assets/dungeons/Assets/obj/floor_tile_small_weeds_B.mtl", "start": 47363694, "end": 47363980}, {"filename": "/assets/dungeons/Assets/obj/floor_tile_small_weeds_B.obj", "start": 47363980, "end": 47402686}, {"filename": "/assets/dungeons/Assets/obj/floor_wood_large.mtl", "start": 47402686, "end": 47402972}, {"filename": "/assets/dungeons/Assets/obj/floor_wood_large.obj", "start": 47402972, "end": 47418059}, {"filename": "/assets/dungeons/Assets/obj/floor_wood_large_dark.mtl", "start": 47418059, "end": 47418345}, {"filename": "/assets/dungeons/Assets/obj/floor_wood_large_dark.obj", "start": 47418345, "end": 47433442}, {"filename": "/assets/dungeons/Assets/obj/floor_wood_small.mtl", "start": 47433442, "end": 47433728}, {"filename": "/assets/dungeons/Assets/obj/floor_wood_small.obj", "start": 47433728, "end": 47438206}, {"filename": "/assets/dungeons/Assets/obj/floor_wood_small_dark.mtl", "start": 47438206, "end": 47438492}, {"filename": "/assets/dungeons/Assets/obj/floor_wood_small_dark.obj", "start": 47438492, "end": 47442980}, {"filename": "/assets/dungeons/Assets/obj/keg.mtl", "start": 47442980, "end": 47443266}, {"filename": "/assets/dungeons/Assets/obj/keg.obj", "start": 47443266, "end": 47508097}, {"filename": "/assets/dungeons/Assets/obj/keg_decorated.mtl", "start": 47508097, "end": 47508383}, {"filename": "/assets/dungeons/Assets/obj/keg_decorated.obj", "start": 47508383, "end": 47687970}, {"filename": "/assets/dungeons/Assets/obj/key.mtl", "start": 47687970, "end": 47688256}, {"filename": "/assets/dungeons/Assets/obj/key.obj", "start": 47688256, "end": 47699545}, {"filename": "/assets/dungeons/Assets/obj/keyring.mtl", "start": 47699545, "end": 47699831}, {"filename": "/assets/dungeons/Assets/obj/keyring.obj", "start": 47699831, "end": 47752716}, {"filename": "/assets/dungeons/Assets/obj/keyring_hanging.mtl", "start": 47752716, "end": 47753002}, {"filename": "/assets/dungeons/Assets/obj/keyring_hanging.obj", "start": 47753002, "end": 47806031}, {"filename": "/assets/dungeons/Assets/obj/pillar.mtl", "start": 47806031, "end": 47806317}, {"filename": "/assets/dungeons/Assets/obj/pillar.obj", "start": 47806317, "end": 47815893}, {"filename": "/assets/dungeons/Assets/obj/pillar_decorated.mtl", "start": 47815893, "end": 47816179}, {"filename": "/assets/dungeons/Assets/obj/pillar_decorated.obj", "start": 47816179, "end": 47884629}, {"filename": "/assets/dungeons/Assets/obj/plate.mtl", "start": 47884629, "end": 47884915}, {"filename": "/assets/dungeons/Assets/obj/plate.obj", "start": 47884915, "end": 47893535}, {"filename": "/assets/dungeons/Assets/obj/plate_food_A.mtl", "start": 47893535, "end": 47893821}, {"filename": "/assets/dungeons/Assets/obj/plate_food_A.obj", "start": 47893821, "end": 47935227}, {"filename": "/assets/dungeons/Assets/obj/plate_food_B.mtl", "start": 47935227, "end": 47935513}, {"filename": "/assets/dungeons/Assets/obj/plate_food_B.obj", "start": 47935513, "end": 47959961}, {"filename": "/assets/dungeons/Assets/obj/plate_small.mtl", "start": 47959961, "end": 47960247}, {"filename": "/assets/dungeons/Assets/obj/plate_small.obj", "start": 47960247, "end": 47968883}, {"filename": "/assets/dungeons/Assets/obj/plate_stack.mtl", "start": 47968883, "end": 47969169}, {"filename": "/assets/dungeons/Assets/obj/plate_stack.obj", "start": 47969169, "end": 47997432}, {"filename": "/assets/dungeons/Assets/obj/rubble_half.mtl", "start": 47997432, "end": 47997718}, {"filename": "/assets/dungeons/Assets/obj/rubble_half.obj", "start": 47997718, "end": 48038783}, {"filename": "/assets/dungeons/Assets/obj/rubble_large.mtl", "start": 48038783, "end": 48039069}, {"filename": "/assets/dungeons/Assets/obj/rubble_large.obj", "start": 48039069, "end": 48123552}, {"filename": "/assets/dungeons/Assets/obj/shelf_large.mtl", "start": 48123552, "end": 48123838}, {"filename": "/assets/dungeons/Assets/obj/shelf_large.obj", "start": 48123838, "end": 48129750}, {"filename": "/assets/dungeons/Assets/obj/shelf_small.mtl", "start": 48129750, "end": 48130036}, {"filename": "/assets/dungeons/Assets/obj/shelf_small.obj", "start": 48130036, "end": 48134666}, {"filename": "/assets/dungeons/Assets/obj/shelf_small_candles.mtl", "start": 48134666, "end": 48134952}, {"filename": "/assets/dungeons/Assets/obj/shelf_small_candles.obj", "start": 48134952, "end": 48154653}, {"filename": "/assets/dungeons/Assets/obj/shelves.mtl", "start": 48154653, "end": 48154939}, {"filename": "/assets/dungeons/Assets/obj/shelves.obj", "start": 48154939, "end": 48166302}, {"filename": "/assets/dungeons/Assets/obj/stairs.mtl", "start": 48166302, "end": 48166588}, {"filename": "/assets/dungeons/Assets/obj/stairs.obj", "start": 48166588, "end": 48199323}, {"filename": "/assets/dungeons/Assets/obj/stairs_long.mtl", "start": 48199323, "end": 48199609}, {"filename": "/assets/dungeons/Assets/obj/stairs_long.obj", "start": 48199609, "end": 48240998}, {"filename": "/assets/dungeons/Assets/obj/stairs_long_modular_center.mtl", "start": 48240998, "end": 48241284}, {"filename": "/assets/dungeons/Assets/obj/stairs_long_modular_center.obj", "start": 48241284, "end": 48246327}, {"filename": "/assets/dungeons/Assets/obj/stairs_long_modular_left.mtl", "start": 48246327, "end": 48246613}, {"filename": "/assets/dungeons/Assets/obj/stairs_long_modular_left.obj", "start": 48246613, "end": 48269863}, {"filename": "/assets/dungeons/Assets/obj/stairs_long_modular_right.mtl", "start": 48269863, "end": 48270149}, {"filename": "/assets/dungeons/Assets/obj/stairs_long_modular_right.obj", "start": 48270149, "end": 48293189}, {"filename": "/assets/dungeons/Assets/obj/stairs_modular_center.mtl", "start": 48293189, "end": 48293475}, {"filename": "/assets/dungeons/Assets/obj/stairs_modular_center.obj", "start": 48293475, "end": 48298508}, {"filename": "/assets/dungeons/Assets/obj/stairs_modular_left.mtl", "start": 48298508, "end": 48298794}, {"filename": "/assets/dungeons/Assets/obj/stairs_modular_left.obj", "start": 48298794, "end": 48317733}, {"filename": "/assets/dungeons/Assets/obj/stairs_modular_right.mtl", "start": 48317733, "end": 48318019}, {"filename": "/assets/dungeons/Assets/obj/stairs_modular_right.obj", "start": 48318019, "end": 48336713}, {"filename": "/assets/dungeons/Assets/obj/stairs_narrow.mtl", "start": 48336713, "end": 48336999}, {"filename": "/assets/dungeons/Assets/obj/stairs_narrow.obj", "start": 48336999, "end": 48369722}, {"filename": "/assets/dungeons/Assets/obj/stairs_wall_left.mtl", "start": 48369722, "end": 48370008}, {"filename": "/assets/dungeons/Assets/obj/stairs_wall_left.obj", "start": 48370008, "end": 48420650}, {"filename": "/assets/dungeons/Assets/obj/stairs_wall_right.mtl", "start": 48420650, "end": 48420936}, {"filename": "/assets/dungeons/Assets/obj/stairs_wall_right.obj", "start": 48420936, "end": 48471583}, {"filename": "/assets/dungeons/Assets/obj/stairs_walled.mtl", "start": 48471583, "end": 48471869}, {"filename": "/assets/dungeons/Assets/obj/stairs_walled.obj", "start": 48471869, "end": 48540127}, {"filename": "/assets/dungeons/Assets/obj/stairs_wide.mtl", "start": 48540127, "end": 48540413}, {"filename": "/assets/dungeons/Assets/obj/stairs_wide.obj", "start": 48540413, "end": 48573156}, {"filename": "/assets/dungeons/Assets/obj/stairs_wood.mtl", "start": 48573156, "end": 48573442}, {"filename": "/assets/dungeons/Assets/obj/stairs_wood.obj", "start": 48573442, "end": 48594229}, {"filename": "/assets/dungeons/Assets/obj/stairs_wood_decorated.mtl", "start": 48594229, "end": 48594515}, {"filename": "/assets/dungeons/Assets/obj/stairs_wood_decorated.obj", "start": 48594515, "end": 48802832}, {"filename": "/assets/dungeons/Assets/obj/stool.mtl", "start": 48802832, "end": 48803118}, {"filename": "/assets/dungeons/Assets/obj/stool.obj", "start": 48803118, "end": 48817717}, {"filename": "/assets/dungeons/Assets/obj/sword_shield.mtl", "start": 48817717, "end": 48818003}, {"filename": "/assets/dungeons/Assets/obj/sword_shield.obj", "start": 48818003, "end": 48867735}, {"filename": "/assets/dungeons/Assets/obj/sword_shield_broken.mtl", "start": 48867735, "end": 48868021}, {"filename": "/assets/dungeons/Assets/obj/sword_shield_broken.obj", "start": 48868021, "end": 48905624}, {"filename": "/assets/dungeons/Assets/obj/sword_shield_gold.mtl", "start": 48905624, "end": 48905910}, {"filename": "/assets/dungeons/Assets/obj/sword_shield_gold.obj", "start": 48905910, "end": 48955652}, {"filename": "/assets/dungeons/Assets/obj/table_long.mtl", "start": 48955652, "end": 48955938}, {"filename": "/assets/dungeons/Assets/obj/table_long.obj", "start": 48955938, "end": 48977932}, {"filename": "/assets/dungeons/Assets/obj/table_long_broken.mtl", "start": 48977932, "end": 48978218}, {"filename": "/assets/dungeons/Assets/obj/table_long_broken.obj", "start": 48978218, "end": 49031606}, {"filename": "/assets/dungeons/Assets/obj/table_long_decorated_A.mtl", "start": 49031606, "end": 49031892}, {"filename": "/assets/dungeons/Assets/obj/table_long_decorated_A.obj", "start": 49031892, "end": 49192164}, {"filename": "/assets/dungeons/Assets/obj/table_long_decorated_C.mtl", "start": 49192164, "end": 49192450}, {"filename": "/assets/dungeons/Assets/obj/table_long_decorated_C.obj", "start": 49192450, "end": 49487989}, {"filename": "/assets/dungeons/Assets/obj/table_long_tablecloth.mtl", "start": 49487989, "end": 49488275}, {"filename": "/assets/dungeons/Assets/obj/table_long_tablecloth.obj", "start": 49488275, "end": 49516569}, {"filename": "/assets/dungeons/Assets/obj/table_long_tablecloth_decorated_A.mtl", "start": 49516569, "end": 49516855}, {"filename": "/assets/dungeons/Assets/obj/table_long_tablecloth_decorated_A.obj", "start": 49516855, "end": 49683936}, {"filename": "/assets/dungeons/Assets/obj/table_medium.mtl", "start": 49683936, "end": 49684222}, {"filename": "/assets/dungeons/Assets/obj/table_medium.obj", "start": 49684222, "end": 49704061}, {"filename": "/assets/dungeons/Assets/obj/table_medium_broken.mtl", "start": 49704061, "end": 49704347}, {"filename": "/assets/dungeons/Assets/obj/table_medium_broken.obj", "start": 49704347, "end": 49728590}, {"filename": "/assets/dungeons/Assets/obj/table_medium_decorated_A.mtl", "start": 49728590, "end": 49728876}, {"filename": "/assets/dungeons/Assets/obj/table_medium_decorated_A.obj", "start": 49728876, "end": 49826282}, {"filename": "/assets/dungeons/Assets/obj/table_medium_tablecloth.mtl", "start": 49826282, "end": 49826568}, {"filename": "/assets/dungeons/Assets/obj/table_medium_tablecloth.obj", "start": 49826568, "end": 49848267}, {"filename": "/assets/dungeons/Assets/obj/table_medium_tablecloth_decorated_B.mtl", "start": 49848267, "end": 49848553}, {"filename": "/assets/dungeons/Assets/obj/table_medium_tablecloth_decorated_B.obj", "start": 49848553, "end": 49947982}, {"filename": "/assets/dungeons/Assets/obj/table_small.mtl", "start": 49947982, "end": 49948268}, {"filename": "/assets/dungeons/Assets/obj/table_small.obj", "start": 49948268, "end": 49964728}, {"filename": "/assets/dungeons/Assets/obj/table_small_decorated_A.mtl", "start": 49964728, "end": 49965014}, {"filename": "/assets/dungeons/Assets/obj/table_small_decorated_A.obj", "start": 49965014, "end": 50041305}, {"filename": "/assets/dungeons/Assets/obj/table_small_decorated_B.mtl", "start": 50041305, "end": 50041591}, {"filename": "/assets/dungeons/Assets/obj/table_small_decorated_B.obj", "start": 50041591, "end": 50156860}, {"filename": "/assets/dungeons/Assets/obj/torch.mtl", "start": 50156860, "end": 50157146}, {"filename": "/assets/dungeons/Assets/obj/torch.obj", "start": 50157146, "end": 50175107}, {"filename": "/assets/dungeons/Assets/obj/torch_lit.mtl", "start": 50175107, "end": 50175393}, {"filename": "/assets/dungeons/Assets/obj/torch_lit.obj", "start": 50175393, "end": 50197095}, {"filename": "/assets/dungeons/Assets/obj/torch_mounted.mtl", "start": 50197095, "end": 50197381}, {"filename": "/assets/dungeons/Assets/obj/torch_mounted.obj", "start": 50197381, "end": 50220272}, {"filename": "/assets/dungeons/Assets/obj/trunk_large_A.mtl", "start": 50220272, "end": 50220558}, {"filename": "/assets/dungeons/Assets/obj/trunk_large_A.obj", "start": 50220558, "end": 50238015}, {"filename": "/assets/dungeons/Assets/obj/trunk_large_B.mtl", "start": 50238015, "end": 50238301}, {"filename": "/assets/dungeons/Assets/obj/trunk_large_B.obj", "start": 50238301, "end": 50255758}, {"filename": "/assets/dungeons/Assets/obj/trunk_large_C.mtl", "start": 50255758, "end": 50256044}, {"filename": "/assets/dungeons/Assets/obj/trunk_large_C.obj", "start": 50256044, "end": 50273501}, {"filename": "/assets/dungeons/Assets/obj/trunk_medium_A.mtl", "start": 50273501, "end": 50273787}, {"filename": "/assets/dungeons/Assets/obj/trunk_medium_A.obj", "start": 50273787, "end": 50286943}, {"filename": "/assets/dungeons/Assets/obj/trunk_medium_B.mtl", "start": 50286943, "end": 50287229}, {"filename": "/assets/dungeons/Assets/obj/trunk_medium_B.obj", "start": 50287229, "end": 50300385}, {"filename": "/assets/dungeons/Assets/obj/trunk_medium_C.mtl", "start": 50300385, "end": 50300671}, {"filename": "/assets/dungeons/Assets/obj/trunk_medium_C.obj", "start": 50300671, "end": 50313827}, {"filename": "/assets/dungeons/Assets/obj/trunk_small_A.mtl", "start": 50313827, "end": 50314113}, {"filename": "/assets/dungeons/Assets/obj/trunk_small_A.obj", "start": 50314113, "end": 50331631}, {"filename": "/assets/dungeons/Assets/obj/trunk_small_B.mtl", "start": 50331631, "end": 50331917}, {"filename": "/assets/dungeons/Assets/obj/trunk_small_B.obj", "start": 50331917, "end": 50349435}, {"filename": "/assets/dungeons/Assets/obj/trunk_small_C.mtl", "start": 50349435, "end": 50349721}, {"filename": "/assets/dungeons/Assets/obj/trunk_small_C.obj", "start": 50349721, "end": 50367239}, {"filename": "/assets/dungeons/Assets/obj/wall.mtl", "start": 50367239, "end": 50367525}, {"filename": "/assets/dungeons/Assets/obj/wall.obj", "start": 50367525, "end": 50411384}, {"filename": "/assets/dungeons/Assets/obj/wall_Tsplit.mtl", "start": 50411384, "end": 50411670}, {"filename": "/assets/dungeons/Assets/obj/wall_Tsplit.obj", "start": 50411670, "end": 50465139}, {"filename": "/assets/dungeons/Assets/obj/wall_Tsplit_sloped.mtl", "start": 50465139, "end": 50465425}, {"filename": "/assets/dungeons/Assets/obj/wall_Tsplit_sloped.obj", "start": 50465425, "end": 50515474}, {"filename": "/assets/dungeons/Assets/obj/wall_arched.mtl", "start": 50515474, "end": 50515760}, {"filename": "/assets/dungeons/Assets/obj/wall_arched.obj", "start": 50515760, "end": 50580333}, {"filename": "/assets/dungeons/Assets/obj/wall_archedwindow_gated.mtl", "start": 50580333, "end": 50580619}, {"filename": "/assets/dungeons/Assets/obj/wall_archedwindow_gated.obj", "start": 50580619, "end": 50644941}, {"filename": "/assets/dungeons/Assets/obj/wall_archedwindow_gated_scaffold.mtl", "start": 50644941, "end": 50645227}, {"filename": "/assets/dungeons/Assets/obj/wall_archedwindow_gated_scaffold.obj", "start": 50645227, "end": 50704695}, {"filename": "/assets/dungeons/Assets/obj/wall_archedwindow_open.mtl", "start": 50704695, "end": 50704981}, {"filename": "/assets/dungeons/Assets/obj/wall_archedwindow_open.obj", "start": 50704981, "end": 50759030}, {"filename": "/assets/dungeons/Assets/obj/wall_broken.mtl", "start": 50759030, "end": 50759316}, {"filename": "/assets/dungeons/Assets/obj/wall_broken.obj", "start": 50759316, "end": 50829670}, {"filename": "/assets/dungeons/Assets/obj/wall_corner.mtl", "start": 50829670, "end": 50829956}, {"filename": "/assets/dungeons/Assets/obj/wall_corner.obj", "start": 50829956, "end": 50869802}, {"filename": "/assets/dungeons/Assets/obj/wall_corner_gated.mtl", "start": 50869802, "end": 50870088}, {"filename": "/assets/dungeons/Assets/obj/wall_corner_gated.obj", "start": 50870088, "end": 50914839}, {"filename": "/assets/dungeons/Assets/obj/wall_corner_scaffold.mtl", "start": 50914839, "end": 50915125}, {"filename": "/assets/dungeons/Assets/obj/wall_corner_scaffold.obj", "start": 50915125, "end": 50963053}, {"filename": "/assets/dungeons/Assets/obj/wall_corner_small.mtl", "start": 50963053, "end": 50963339}, {"filename": "/assets/dungeons/Assets/obj/wall_corner_small.obj", "start": 50963339, "end": 50983801}, {"filename": "/assets/dungeons/Assets/obj/wall_cracked.mtl", "start": 50983801, "end": 50984087}, {"filename": "/assets/dungeons/Assets/obj/wall_cracked.obj", "start": 50984087, "end": 51170625}, {"filename": "/assets/dungeons/Assets/obj/wall_crossing.mtl", "start": 51170625, "end": 51170911}, {"filename": "/assets/dungeons/Assets/obj/wall_crossing.obj", "start": 51170911, "end": 51232773}, {"filename": "/assets/dungeons/Assets/obj/wall_doorway.mtl", "start": 51232773, "end": 51233059}, {"filename": "/assets/dungeons/Assets/obj/wall_doorway.obj", "start": 51233059, "end": 51270299}, {"filename": "/assets/dungeons/Assets/obj/wall_doorway_Tsplit.mtl", "start": 51270299, "end": 51270585}, {"filename": "/assets/dungeons/Assets/obj/wall_doorway_Tsplit.obj", "start": 51270585, "end": 51367043}, {"filename": "/assets/dungeons/Assets/obj/wall_doorway_door.mtl", "start": 51367043, "end": 51367329}, {"filename": "/assets/dungeons/Assets/obj/wall_doorway_door.obj", "start": 51367329, "end": 51412756}, {"filename": "/assets/dungeons/Assets/obj/wall_doorway_scaffold.mtl", "start": 51412756, "end": 51413042}, {"filename": "/assets/dungeons/Assets/obj/wall_doorway_scaffold.obj", "start": 51413042, "end": 51459570}, {"filename": "/assets/dungeons/Assets/obj/wall_doorway_scaffold_door.mtl", "start": 51459570, "end": 51459856}, {"filename": "/assets/dungeons/Assets/obj/wall_doorway_scaffold_door.obj", "start": 51459856, "end": 51505301}, {"filename": "/assets/dungeons/Assets/obj/wall_doorway_sides.mtl", "start": 51505301, "end": 51505587}, {"filename": "/assets/dungeons/Assets/obj/wall_doorway_sides.obj", "start": 51505587, "end": 51608419}, {"filename": "/assets/dungeons/Assets/obj/wall_endcap.mtl", "start": 51608419, "end": 51608705}, {"filename": "/assets/dungeons/Assets/obj/wall_endcap.obj", "start": 51608705, "end": 51629725}, {"filename": "/assets/dungeons/Assets/obj/wall_gated.mtl", "start": 51629725, "end": 51630011}, {"filename": "/assets/dungeons/Assets/obj/wall_gated.obj", "start": 51630011, "end": 51667623}, {"filename": "/assets/dungeons/Assets/obj/wall_half.mtl", "start": 51667623, "end": 51667909}, {"filename": "/assets/dungeons/Assets/obj/wall_half.obj", "start": 51667909, "end": 51691739}, {"filename": "/assets/dungeons/Assets/obj/wall_half_endcap.mtl", "start": 51691739, "end": 51692025}, {"filename": "/assets/dungeons/Assets/obj/wall_half_endcap.obj", "start": 51692025, "end": 51716038}, {"filename": "/assets/dungeons/Assets/obj/wall_half_endcap_sloped.mtl", "start": 51716038, "end": 51716324}, {"filename": "/assets/dungeons/Assets/obj/wall_half_endcap_sloped.obj", "start": 51716324, "end": 51736343}, {"filename": "/assets/dungeons/Assets/obj/wall_open_scaffold.mtl", "start": 51736343, "end": 51736629}, {"filename": "/assets/dungeons/Assets/obj/wall_open_scaffold.obj", "start": 51736629, "end": 51751752}, {"filename": "/assets/dungeons/Assets/obj/wall_pillar.mtl", "start": 51751752, "end": 51752038}, {"filename": "/assets/dungeons/Assets/obj/wall_pillar.obj", "start": 51752038, "end": 51798554}, {"filename": "/assets/dungeons/Assets/obj/wall_scaffold.mtl", "start": 51798554, "end": 51798840}, {"filename": "/assets/dungeons/Assets/obj/wall_scaffold.obj", "start": 51798840, "end": 51852365}, {"filename": "/assets/dungeons/Assets/obj/wall_shelves.mtl", "start": 51852365, "end": 51852651}, {"filename": "/assets/dungeons/Assets/obj/wall_shelves.obj", "start": 51852651, "end": 51992692}, {"filename": "/assets/dungeons/Assets/obj/wall_sloped.mtl", "start": 51992692, "end": 51992978}, {"filename": "/assets/dungeons/Assets/obj/wall_sloped.obj", "start": 51992978, "end": 52016164}, {"filename": "/assets/dungeons/Assets/obj/wall_window_closed.mtl", "start": 52016164, "end": 52016450}, {"filename": "/assets/dungeons/Assets/obj/wall_window_closed.obj", "start": 52016450, "end": 52072565}, {"filename": "/assets/dungeons/Assets/obj/wall_window_closed_scaffold.mtl", "start": 52072565, "end": 52072851}, {"filename": "/assets/dungeons/Assets/obj/wall_window_closed_scaffold.obj", "start": 52072851, "end": 52137165}, {"filename": "/assets/dungeons/Assets/obj/wall_window_open.mtl", "start": 52137165, "end": 52137451}, {"filename": "/assets/dungeons/Assets/obj/wall_window_open.obj", "start": 52137451, "end": 52187634}, {"filename": "/assets/dungeons/Assets/obj/wall_window_open_scaffold.mtl", "start": 52187634, "end": 52187920}, {"filename": "/assets/dungeons/Assets/obj/wall_window_open_scaffold.obj", "start": 52187920, "end": 52246270}, {"filename": "/assets/dungeons/Assets/textures/dungeon_texture.png", "start": 52246270, "end": 52263317}, {"filename": "/assets/dungeons/Discord Server.url", "start": 52263317, "end": 52263436}, {"filename": "/assets/dungeons/License.txt", "start": 52263436, "end": 52264509}, {"filename": "/assets/dungeons/More KayKit Assets.url", "start": 52264509, "end": 52264637}, {"filename": "/assets/dungeons/Patreon.url", "start": 52264637, "end": 52264762}, {"filename": "/assets/dungeons/Samples/dungeon_remastered_sample1.png", "start": 52264762, "end": 53284911}, {"filename": "/assets/dungeons/Samples/dungeon_remastered_sample10.png", "start": 53284911, "end": 54476045}, {"filename": "/assets/dungeons/Samples/dungeon_remastered_sample11.png", "start": 54476045, "end": 55819446}, {"filename": "/assets/dungeons/Samples/dungeon_remastered_sample2.png", "start": 55819446, "end": 57066907}, {"filename": "/assets/dungeons/Samples/dungeon_remastered_sample3.png", "start": 57066907, "end": 58218112}, {"filename": "/assets/dungeons/Samples/dungeon_remastered_sample4.png", "start": 58218112, "end": 59559031}, {"filename": "/assets/dungeons/Samples/dungeon_remastered_sample5.png", "start": 59559031, "end": 60607993}, {"filename": "/assets/dungeons/Samples/dungeon_remastered_sample6.png", "start": 60607993, "end": 62171710}, {"filename": "/assets/dungeons/Samples/dungeon_remastered_sample7.png", "start": 62171710, "end": 63638298}, {"filename": "/assets/dungeons/Samples/dungeon_remastered_sample8.png", "start": 63638298, "end": 64593538}, {"filename": "/assets/dungeons/Samples/dungeon_remastered_sample9.png", "start": 64593538, "end": 65831133}, {"filename": "/assets/dungeons/Samples/dungeon_remastered_sample_big.png", "start": 65831133, "end": 66937145}, {"filename": "/assets/dungeons/Samples/extra_content1.png", "start": 66937145, "end": 68456345}, {"filename": "/assets/dungeons/Samples/extra_content2.png", "start": 68456345, "end": 69489300}, {"filename": "/assets/dungeons/Samples/source_content.png", "start": 69489300, "end": 71600594}, {"filename": "/assets/dungeons/contents.png", "start": 71600594, "end": 72574914}, {"filename": "/assets/font.ttf", "start": 72574914, "end": 73109130}, {"filename": "/assets/grapple.wav", "start": 73109130, "end": 73122090, "audio": 1}, {"filename": "/assets/hit.wav", "start": 73122090, "end": 73150934, "audio": 1}, {"filename": "/assets/jump.wav", "start": 73150934, "end": 73203128, "audio": 1}, {"filename": "/assets/shoot.wav", "start": 73203128, "end": 73295328, "audio": 1}, {"filename": "/assets/skeletons/Animations/Click here for more Free Animations.url", "start": 73295328, "end": 73295483}, {"filename": "/assets/skeletons/Animations/fbx/Rig_Medium/Rig_Medium_General.fbx", "start": 73295483, "end": 75678183}, {"filename": "/assets/skeletons/Animations/fbx/Rig_Medium/Rig_Medium_MovementBasic.fbx", "start": 75678183, "end": 77499251}, {"filename": "/assets/skeletons/Animations/gltf/Rig_Medium/Rig_Medium_General.glb", "start": 77499251, "end": 78327491}, {"filename": "/assets/skeletons/Animations/gltf/Rig_Medium/Rig_Medium_MovementBasic.glb", "start": 78327491, "end": 79017115}, {"filename": "/assets/skeletons/Discord Server.url", "start": 79017115, "end": 79017234}, {"filename": "/assets/skeletons/License.txt", "start": 79017234, "end": 79018786}, {"filename": "/assets/skeletons/More KayKit Assets.url", "start": 79018786, "end": 79018914}, {"filename": "/assets/skeletons/Patreon.url", "start": 79018914, "end": 79019039}, {"filename": "/assets/skeletons/assets/fbx(unity)/Skeleton_Arrow.fbx", "start": 79019039, "end": 79039563}, {"filename": "/assets/skeletons/assets/fbx(unity)/Skeleton_Arrow_Broken.fbx", "start": 79039563, "end": 79060631}, {"filename": "/assets/skeletons/assets/fbx(unity)/Skeleton_Arrow_Broken_Half.fbx", "start": 79060631, "end": 79081075}, {"filename": "/assets/skeletons/assets/fbx(unity)/Skeleton_Arrow_Half.fbx", "start": 79081075, "end": 79101151}, {"filename": "/assets/skeletons/assets/fbx(unity)/Skeleton_Axe.fbx", "start": 79101151, "end": 79138555}, {"filename": "/assets/skeletons/assets/fbx(unity)/Skeleton_Blade.fbx", "start": 79138555, "end": 79173015}, {"filename": "/assets/skeletons/assets/fbx(unity)/Skeleton_Crossbow.fbx", "start": 79173015, "end": 79212611}, {"filename": "/assets/skeletons/assets/fbx(unity)/Skeleton_Quiver.fbx", "start": 79212611, "end": 79243919}, {"filename": "/assets/skeletons/assets/fbx(unity)/Skeleton_Shield_Large_A.fbx", "start": 79243919, "end": 79287947}, {"filename": "/assets/skeletons/assets/fbx(unity)/Skeleton_Shield_Large_B.fbx", "start": 79287947, "end": 79318471}, {"filename": "/assets/skeletons/assets/fbx(unity)/Skeleton_Shield_Small_A.fbx", "start": 79318471, "end": 79353443}, {"filename": "/assets/skeletons/assets/fbx(unity)/Skeleton_Shield_Small_B.fbx", "start": 79353443, "end": 79392431}, {"filename": "/assets/skeletons/assets/fbx(unity)/Skeleton_Staff.fbx", "start": 79392431, "end": 79470219}, {"filename": "/assets/skeletons/assets/fbx(unity)/skeleton_texture.png", "start": 79470219, "end": 79487256}, {"filename": "/assets/skeletons/assets/fbx/Skeleton_Arrow.fbx", "start": 79487256, "end": 79507780}, {"filename": "/assets/skeletons/assets/fbx/Skeleton_Arrow_Broken.fbx", "start": 79507780, "end": 79528832}, {"filename": "/assets/skeletons/assets/fbx/Skeleton_Arrow_Broken_Half.fbx", "start": 79528832, "end": 79549276}, {"filename": "/assets/skeletons/assets/fbx/Skeleton_Arrow_Half.fbx", "start": 79549276, "end": 79569304}, {"filename": "/assets/skeletons/assets/fbx/Skeleton_Axe.fbx", "start": 79569304, "end": 79606628}, {"filename": "/assets/skeletons/assets/fbx/Skeleton_Blade.fbx", "start": 79606628, "end": 79641136}, {"filename": "/assets/skeletons/assets/fbx/Skeleton_Crossbow.fbx", "start": 79641136, "end": 79680684}, {"filename": "/assets/skeletons/assets/fbx/Skeleton_Quiver.fbx", "start": 79680684, "end": 79711976}, {"filename": "/assets/skeletons/assets/fbx/Skeleton_Shield_Large_A.fbx", "start": 79711976, "end": 79755892}, {"filename": "/assets/skeletons/assets/fbx/Skeleton_Shield_Large_B.fbx", "start": 79755892, "end": 79786096}, {"filename": "/assets/skeletons/assets/fbx/Skeleton_Shield_Small_A.fbx", "start": 79786096, "end": 79821100}, {"filename": "/assets/skeletons/assets/fbx/Skeleton_Shield_Small_B.fbx", "start": 79821100, "end": 79860136}, {"filename": "/assets/skeletons/assets/fbx/Skeleton_Staff.fbx", "start": 79860136, "end": 79937972}, {"filename": "/assets/skeletons/assets/fbx/skeleton_texture.png", "start": 79937972, "end": 79955009}, {"filename": "/assets/skeletons/assets/gltf/Skeleton_Arrow.bin", "start": 79955009, "end": 79957657}, {"filename": "/assets/skeletons/assets/gltf/Skeleton_Arrow.gltf", "start": 79957657, "end": 79960700}, {"filename": "/assets/skeletons/assets/gltf/Skeleton_Arrow_Broken.bin", "start": 79960700, "end": 79964236}, {"filename": "/assets/skeletons/assets/gltf/Skeleton_Arrow_Broken.gltf", "start": 79964236, "end": 79967297}, {"filename": "/assets/skeletons/assets/gltf/Skeleton_Arrow_Broken_Half.bin", "start": 79967297, "end": 79970117}, {"filename": "/assets/skeletons/assets/gltf/Skeleton_Arrow_Broken_Half.gltf", "start": 79970117, "end": 79973186}, {"filename": "/assets/skeletons/assets/gltf/Skeleton_Arrow_Half.bin", "start": 79973186, "end": 79975474}, {"filename": "/assets/skeletons/assets/gltf/Skeleton_Arrow_Half.gltf", "start": 79975474, "end": 79978525}, {"filename": "/assets/skeletons/assets/gltf/Skeleton_Axe.bin", "start": 79978525, "end": 80003873}, {"filename": "/assets/skeletons/assets/gltf/Skeleton_Axe.gltf", "start": 80003873, "end": 80006918}, {"filename": "/assets/skeletons/assets/gltf/Skeleton_Blade.bin", "start": 80006918, "end": 80026974}, {"filename": "/assets/skeletons/assets/gltf/Skeleton_Blade.gltf", "start": 80026974, "end": 80030021}, {"filename": "/assets/skeletons/assets/gltf/Skeleton_Crossbow.bin", "start": 80030021, "end": 80058645}, {"filename": "/assets/skeletons/assets/gltf/Skeleton_Crossbow.gltf", "start": 80058645, "end": 80061697}, {"filename": "/assets/skeletons/assets/gltf/Skeleton_Quiver.bin", "start": 80061697, "end": 80080317}, {"filename": "/assets/skeletons/assets/gltf/Skeleton_Quiver.gltf", "start": 80080317, "end": 80083368}, {"filename": "/assets/skeletons/assets/gltf/Skeleton_Shield_Large_A.bin", "start": 80083368, "end": 80116660}, {"filename": "/assets/skeletons/assets/gltf/Skeleton_Shield_Large_A.gltf", "start": 80116660, "end": 80119731}, {"filename": "/assets/skeletons/assets/gltf/Skeleton_Shield_Large_B.bin", "start": 80119731, "end": 80138551}, {"filename": "/assets/skeletons/assets/gltf/Skeleton_Shield_Large_B.gltf", "start": 80138551, "end": 80141619}, {"filename": "/assets/skeletons/assets/gltf/Skeleton_Shield_Small_A.bin", "start": 80141619, "end": 80163307}, {"filename": "/assets/skeletons/assets/gltf/Skeleton_Shield_Small_A.gltf", "start": 80163307, "end": 80166376}, {"filename": "/assets/skeletons/assets/gltf/Skeleton_Shield_Small_B.bin", "start": 80166376, "end": 80195012}, {"filename": "/assets/skeletons/assets/gltf/Skeleton_Shield_Small_B.gltf", "start": 80195012, "end": 80198081}, {"filename": "/assets/skeletons/assets/gltf/Skeleton_Staff.bin", "start": 80198081, "end": 80267325}, {"filename": "/assets/skeletons/assets/gltf/Skeleton_Staff.gltf", "start": 80267325, "end": 80270382}, {"filename": "/assets/skeletons/assets/gltf/skeleton_texture.png", "start": 80270382, "end": 80287419}, {"filename": "/assets/skeletons/assets/obj/Skeleton_Arrow.mtl", "start": 80287419, "end": 80287705}, {"filename": "/assets/skeletons/assets/obj/Skeleton_Arrow.obj", "start": 80287705, "end": 80292439}, {"filename": "/assets/skeletons/assets/obj/Skeleton_Arrow_Broken.mtl", "start": 80292439, "end": 80292725}, {"filename": "/assets/skeletons/assets/obj/Skeleton_Arrow_Broken.obj", "start": 80292725, "end": 80298694}, {"filename": "/assets/skeletons/assets/obj/Skeleton_Arrow_Broken_Half.mtl", "start": 80298694, "end": 80298980}, {"filename": "/assets/skeletons/assets/obj/Skeleton_Arrow_Broken_Half.obj", "start": 80298980, "end": 80303511}, {"filename": "/assets/skeletons/assets/obj/Skeleton_Arrow_Half.mtl", "start": 80303511, "end": 80303797}, {"filename": "/assets/skeletons/assets/obj/Skeleton_Arrow_Half.obj", "start": 80303797, "end": 80307434}, {"filename": "/assets/skeletons/assets/obj/Skeleton_Axe.mtl", "start": 80307434, "end": 80307720}, {"filename": "/assets/skeletons/assets/obj/Skeleton_Axe.obj", "start": 80307720, "end": 80351857}, {"filename": "/assets/skeletons/assets/obj/Skeleton_Blade.mtl", "start": 80351857, "end": 80352143}, {"filename": "/assets/skeletons/assets/obj/Skeleton_Blade.obj", "start": 80352143, "end": 80390176}, {"filename": "/assets/skeletons/assets/obj/Skeleton_Crossbow.mtl", "start": 80390176, "end": 80390462}, {"filename": "/assets/skeletons/assets/obj/Skeleton_Crossbow.obj", "start": 80390462, "end": 80441497}, {"filename": "/assets/skeletons/assets/obj/Skeleton_Quiver.mtl", "start": 80441497, "end": 80441783}, {"filename": "/assets/skeletons/assets/obj/Skeleton_Quiver.obj", "start": 80441783, "end": 80472938}, {"filename": "/assets/skeletons/assets/obj/Skeleton_Shield_Large_A.mtl", "start": 80472938, "end": 80473224}, {"filename": "/assets/skeletons/assets/obj/Skeleton_Shield_Large_A.obj", "start": 80473224, "end": 80532875}, {"filename": "/assets/skeletons/assets/obj/Skeleton_Shield_Large_B.mtl", "start": 80532875, "end": 80533161}, {"filename": "/assets/skeletons/assets/obj/Skeleton_Shield_Large_B.obj", "start": 80533161, "end": 80563304}, {"filename": "/assets/skeletons/assets/obj/Skeleton_Shield_Small_A.mtl", "start": 80563304, "end": 80563590}, {"filename": "/assets/skeletons/assets/obj/Skeleton_Shield_Small_A.obj", "start": 80563590, "end": 80605323}, {"filename": "/assets/skeletons/assets/obj/Skeleton_Shield_Small_B.mtl", "start": 80605323, "end": 80605609}, {"filename": "/assets/skeletons/assets/obj/Skeleton_Shield_Small_B.obj", "start": 80605609, "end": 80657857}, {"filename": "/assets/skeletons/assets/obj/Skeleton_Staff.mtl", "start": 80657857, "end": 80658143}, {"filename": "/assets/skeletons/assets/obj/Skeleton_Staff.obj", "start": 80658143, "end": 80793567}, {"filename": "/assets/skeletons/assets/obj/skeleton_texture.png", "start": 80793567, "end": 80810604}, {"filename": "/assets/skeletons/characters/fbx/Skeleton_Mage.fbx", "start": 80810604, "end": 81217416}, {"filename": "/assets/skeletons/characters/fbx/Skeleton_Minion.fbx", "start": 81217416, "end": 81679028}, {"filename": "/assets/skeletons/characters/fbx/Skeleton_Rogue.fbx", "start": 81679028, "end": 82128208}, {"filename": "/assets/skeletons/characters/fbx/Skeleton_Warrior.fbx", "start": 82128208, "end": 82625820}, {"filename": "/assets/skeletons/characters/fbx/skeleton_texture.png", "start": 82625820, "end": 82642857}, {"filename": "/assets/skeletons/characters/gltf/Skeleton_Mage.glb", "start": 82642857, "end": 82908533}, {"filename": "/assets/skeletons/characters/gltf/Skeleton_Minion.glb", "start": 82908533, "end": 83227053}, {"filename": "/assets/skeletons/characters/gltf/Skeleton_Rogue.glb", "start": 83227053, "end": 83558233}, {"filename": "/assets/skeletons/characters/gltf/Skeleton_Warrior.glb", "start": 83558233, "end": 83925717}, {"filename": "/assets/skeletons/characters/gltf/skeleton_texture.png", "start": 83925717, "end": 83942754}, {"filename": "/assets/skeletons/contents.png", "start": 83942754, "end": 85497963}, {"filename": "/assets/skeletons/samples/mage.png", "start": 85497963, "end": 87048196}, {"filename": "/assets/skeletons/samples/minion.png", "start": 87048196, "end": 87757612}, {"filename": "/assets/skeletons/samples/rogue.png", "start": 87757612, "end": 88507239}, {"filename": "/assets/skeletons/samples/warrior.png", "start": 88507239, "end": 90149737}, {"filename": "/assets/skeletons/texture/skeleton_texture.png", "start": 90149737, "end": 90166774}], "remote_package_size": 90166774});

  })();


    // All the pre-js content up to here must remain later on, we need to run
    // it.
    if (Module['ENVIRONMENT_IS_PTHREAD'] || Module['$ww']) Module['preRun'] = [];
    var necessaryPreJSTasks = Module['preRun'].slice();
  
    if (!Module['preRun']) throw 'Module.preRun should exist because file support used it; did a pre-js delete it?';
    necessaryPreJSTasks.forEach(function(task) {
      if (Module['preRun'].indexOf(task) < 0) throw 'All preRun tasks that exist before user pre-js code should remain after; did you replace Module or modify Module.preRun?';
    });
  

// Sometimes an existing Module object exists with properties
// meant to overwrite the default module functionality. Here
// we collect those properties and reapply _after_ we configure
// the current environment's defaults to avoid having to be so
// defensive during initialization.
var moduleOverrides = Object.assign({}, Module);

var arguments_ = [];
var thisProgram = './this.program';
var quit_ = (status, toThrow) => {
  throw toThrow;
};

// Determine the runtime environment we are in. You can customize this by
// setting the ENVIRONMENT setting at compile time (see settings.js).

// Attempt to auto-detect the environment
var ENVIRONMENT_IS_WEB = typeof window == 'object';
var ENVIRONMENT_IS_WORKER = typeof importScripts == 'function';
// N.b. Electron.js environment is simultaneously a NODE-environment, but
// also a web environment.
var ENVIRONMENT_IS_NODE = typeof process == 'object' && typeof process.versions == 'object' && typeof process.versions.node == 'string';
var ENVIRONMENT_IS_SHELL = !ENVIRONMENT_IS_WEB && !ENVIRONMENT_IS_NODE && !ENVIRONMENT_IS_WORKER;

if (Module['ENVIRONMENT']) {
  throw new Error('Module.ENVIRONMENT has been deprecated. To force the environment, use the ENVIRONMENT compile-time option (for example, -sENVIRONMENT=web or -sENVIRONMENT=node)');
}

// `/` should be present at the end if `scriptDirectory` is not empty
var scriptDirectory = '';
function locateFile(path) {
  if (Module['locateFile']) {
    return Module['locateFile'](path, scriptDirectory);
  }
  return scriptDirectory + path;
}

// Hooks that are implemented differently in different runtime environments.
var read_,
    readAsync,
    readBinary,
    setWindowTitle;

if (ENVIRONMENT_IS_NODE) {
  if (typeof process == 'undefined' || !process.release || process.release.name !== 'node') throw new Error('not compiled for this environment (did you build to HTML and try to run it not on the web, or set ENVIRONMENT to something - like node - and run it someplace else - like on the web?)');

  var nodeVersion = process.versions.node;
  var numericVersion = nodeVersion.split('.').slice(0, 3);
  numericVersion = (numericVersion[0] * 10000) + (numericVersion[1] * 100) + (numericVersion[2].split('-')[0] * 1);
  var minVersion = 160000;
  if (numericVersion < 160000) {
    throw new Error('This emscripten-generated code requires node v16.0.0 (detected v' + nodeVersion + ')');
  }

  // `require()` is no-op in an ESM module, use `createRequire()` to construct
  // the require()` function.  This is only necessary for multi-environment
  // builds, `-sENVIRONMENT=node` emits a static import declaration instead.
  // TODO: Swap all `require()`'s with `import()`'s?
  // These modules will usually be used on Node.js. Load them eagerly to avoid
  // the complexity of lazy-loading.
  var fs = require('fs');
  var nodePath = require('path');

  if (ENVIRONMENT_IS_WORKER) {
    scriptDirectory = nodePath.dirname(scriptDirectory) + '/';
  } else {
    scriptDirectory = __dirname + '/';
  }

// include: node_shell_read.js
read_ = (filename, binary) => {
  // We need to re-wrap `file://` strings to URLs. Normalizing isn't
  // necessary in that case, the path should already be absolute.
  filename = isFileURI(filename) ? new URL(filename) : nodePath.normalize(filename);
  return fs.readFileSync(filename, binary ? undefined : 'utf8');
};

readBinary = (filename) => {
  var ret = read_(filename, true);
  if (!ret.buffer) {
    ret = new Uint8Array(ret);
  }
  assert(ret.buffer);
  return ret;
};

readAsync = (filename, onload, onerror, binary = true) => {
  // See the comment in the `read_` function.
  filename = isFileURI(filename) ? new URL(filename) : nodePath.normalize(filename);
  fs.readFile(filename, binary ? undefined : 'utf8', (err, data) => {
    if (err) onerror(err);
    else onload(binary ? data.buffer : data);
  });
};
// end include: node_shell_read.js
  if (!Module['thisProgram'] && process.argv.length > 1) {
    thisProgram = process.argv[1].replace(/\\/g, '/');
  }

  arguments_ = process.argv.slice(2);

  if (typeof module != 'undefined') {
    module['exports'] = Module;
  }

  process.on('uncaughtException', (ex) => {
    // suppress ExitStatus exceptions from showing an error
    if (ex !== 'unwind' && !(ex instanceof ExitStatus) && !(ex.context instanceof ExitStatus)) {
      throw ex;
    }
  });

  quit_ = (status, toThrow) => {
    process.exitCode = status;
    throw toThrow;
  };

  Module['inspect'] = () => '[Emscripten Module object]';

} else
if (ENVIRONMENT_IS_SHELL) {

  if ((typeof process == 'object' && typeof require === 'function') || typeof window == 'object' || typeof importScripts == 'function') throw new Error('not compiled for this environment (did you build to HTML and try to run it not on the web, or set ENVIRONMENT to something - like node - and run it someplace else - like on the web?)');

  if (typeof read != 'undefined') {
    read_ = read;
  }

  readBinary = (f) => {
    if (typeof readbuffer == 'function') {
      return new Uint8Array(readbuffer(f));
    }
    let data = read(f, 'binary');
    assert(typeof data == 'object');
    return data;
  };

  readAsync = (f, onload, onerror) => {
    setTimeout(() => onload(readBinary(f)));
  };

  if (typeof clearTimeout == 'undefined') {
    globalThis.clearTimeout = (id) => {};
  }

  if (typeof setTimeout == 'undefined') {
    // spidermonkey lacks setTimeout but we use it above in readAsync.
    globalThis.setTimeout = (f) => (typeof f == 'function') ? f() : abort();
  }

  if (typeof scriptArgs != 'undefined') {
    arguments_ = scriptArgs;
  } else if (typeof arguments != 'undefined') {
    arguments_ = arguments;
  }

  if (typeof quit == 'function') {
    quit_ = (status, toThrow) => {
      // Unlike node which has process.exitCode, d8 has no such mechanism. So we
      // have no way to set the exit code and then let the program exit with
      // that code when it naturally stops running (say, when all setTimeouts
      // have completed). For that reason, we must call `quit` - the only way to
      // set the exit code - but quit also halts immediately.  To increase
      // consistency with node (and the web) we schedule the actual quit call
      // using a setTimeout to give the current stack and any exception handlers
      // a chance to run.  This enables features such as addOnPostRun (which
      // expected to be able to run code after main returns).
      setTimeout(() => {
        if (!(toThrow instanceof ExitStatus)) {
          let toLog = toThrow;
          if (toThrow && typeof toThrow == 'object' && toThrow.stack) {
            toLog = [toThrow, toThrow.stack];
          }
          err(`exiting due to exception: ${toLog}`);
        }
        quit(status);
      });
      throw toThrow;
    };
  }

  if (typeof print != 'undefined') {
    // Prefer to use print/printErr where they exist, as they usually work better.
    if (typeof console == 'undefined') console = /** @type{!Console} */({});
    console.log = /** @type{!function(this:Console, ...*): undefined} */ (print);
    console.warn = console.error = /** @type{!function(this:Console, ...*): undefined} */ (typeof printErr != 'undefined' ? printErr : print);
  }

} else

// Note that this includes Node.js workers when relevant (pthreads is enabled).
// Node.js workers are detected as a combination of ENVIRONMENT_IS_WORKER and
// ENVIRONMENT_IS_NODE.
if (ENVIRONMENT_IS_WEB || ENVIRONMENT_IS_WORKER) {
  if (ENVIRONMENT_IS_WORKER) { // Check worker, not web, since window could be polyfilled
    scriptDirectory = self.location.href;
  } else if (typeof document != 'undefined' && document.currentScript) { // web
    scriptDirectory = document.currentScript.src;
  }
  // blob urls look like blob:http://site.com/etc/etc and we cannot infer anything from them.
  // otherwise, slice off the final part of the url to find the script directory.
  // if scriptDirectory does not contain a slash, lastIndexOf will return -1,
  // and scriptDirectory will correctly be replaced with an empty string.
  // If scriptDirectory contains a query (starting with ?) or a fragment (starting with #),
  // they are removed because they could contain a slash.
  if (scriptDirectory.indexOf('blob:') !== 0) {
    scriptDirectory = scriptDirectory.substr(0, scriptDirectory.replace(/[?#].*/, "").lastIndexOf('/')+1);
  } else {
    scriptDirectory = '';
  }

  if (!(typeof window == 'object' || typeof importScripts == 'function')) throw new Error('not compiled for this environment (did you build to HTML and try to run it not on the web, or set ENVIRONMENT to something - like node - and run it someplace else - like on the web?)');

  // Differentiate the Web Worker from the Node Worker case, as reading must
  // be done differently.
  {
// include: web_or_worker_shell_read.js
read_ = (url) => {
    var xhr = new XMLHttpRequest();
    xhr.open('GET', url, false);
    xhr.send(null);
    return xhr.responseText;
  }

  if (ENVIRONMENT_IS_WORKER) {
    readBinary = (url) => {
      var xhr = new XMLHttpRequest();
      xhr.open('GET', url, false);
      xhr.responseType = 'arraybuffer';
      xhr.send(null);
      return new Uint8Array(/** @type{!ArrayBuffer} */(xhr.response));
    };
  }

  readAsync = (url, onload, onerror) => {
    var xhr = new XMLHttpRequest();
    xhr.open('GET', url, true);
    xhr.responseType = 'arraybuffer';
    xhr.onload = () => {
      if (xhr.status == 200 || (xhr.status == 0 && xhr.response)) { // file URLs can return 0
        onload(xhr.response);
        return;
      }
      onerror();
    };
    xhr.onerror = onerror;
    xhr.send(null);
  }

// end include: web_or_worker_shell_read.js
  }

  setWindowTitle = (title) => document.title = title;
} else
{
  throw new Error('environment detection error');
}

var out = Module['print'] || console.log.bind(console);
var err = Module['printErr'] || console.error.bind(console);

// Merge back in the overrides
Object.assign(Module, moduleOverrides);
// Free the object hierarchy contained in the overrides, this lets the GC
// reclaim data used e.g. in memoryInitializerRequest, which is a large typed array.
moduleOverrides = null;
checkIncomingModuleAPI();

// Emit code to handle expected values on the Module object. This applies Module.x
// to the proper local x. This has two benefits: first, we only emit it if it is
// expected to arrive, and second, by using a local everywhere else that can be
// minified.

if (Module['arguments']) arguments_ = Module['arguments'];legacyModuleProp('arguments', 'arguments_');

if (Module['thisProgram']) thisProgram = Module['thisProgram'];legacyModuleProp('thisProgram', 'thisProgram');

if (Module['quit']) quit_ = Module['quit'];legacyModuleProp('quit', 'quit_');

// perform assertions in shell.js after we set up out() and err(), as otherwise if an assertion fails it cannot print the message
// Assertions on removed incoming Module JS APIs.
assert(typeof Module['memoryInitializerPrefixURL'] == 'undefined', 'Module.memoryInitializerPrefixURL option was removed, use Module.locateFile instead');
assert(typeof Module['pthreadMainPrefixURL'] == 'undefined', 'Module.pthreadMainPrefixURL option was removed, use Module.locateFile instead');
assert(typeof Module['cdInitializerPrefixURL'] == 'undefined', 'Module.cdInitializerPrefixURL option was removed, use Module.locateFile instead');
assert(typeof Module['filePackagePrefixURL'] == 'undefined', 'Module.filePackagePrefixURL option was removed, use Module.locateFile instead');
assert(typeof Module['read'] == 'undefined', 'Module.read option was removed (modify read_ in JS)');
assert(typeof Module['readAsync'] == 'undefined', 'Module.readAsync option was removed (modify readAsync in JS)');
assert(typeof Module['readBinary'] == 'undefined', 'Module.readBinary option was removed (modify readBinary in JS)');
assert(typeof Module['setWindowTitle'] == 'undefined', 'Module.setWindowTitle option was removed (modify setWindowTitle in JS)');
assert(typeof Module['TOTAL_MEMORY'] == 'undefined', 'Module.TOTAL_MEMORY has been renamed Module.INITIAL_MEMORY');
legacyModuleProp('asm', 'wasmExports');
legacyModuleProp('read', 'read_');
legacyModuleProp('readAsync', 'readAsync');
legacyModuleProp('readBinary', 'readBinary');
legacyModuleProp('setWindowTitle', 'setWindowTitle');
var IDBFS = 'IDBFS is no longer included by default; build with -lidbfs.js';
var PROXYFS = 'PROXYFS is no longer included by default; build with -lproxyfs.js';
var WORKERFS = 'WORKERFS is no longer included by default; build with -lworkerfs.js';
var NODEFS = 'NODEFS is no longer included by default; build with -lnodefs.js';

assert(!ENVIRONMENT_IS_SHELL, "shell environment detected but not enabled at build time.  Add 'shell' to `-sENVIRONMENT` to enable.");


// end include: shell.js
// include: preamble.js
// === Preamble library stuff ===

// Documentation for the public APIs defined in this file must be updated in:
//    site/source/docs/api_reference/preamble.js.rst
// A prebuilt local version of the documentation is available at:
//    site/build/text/docs/api_reference/preamble.js.txt
// You can also build docs locally as HTML or other formats in site/
// An online HTML version (which may be of a different version of Emscripten)
//    is up at http://kripken.github.io/emscripten-site/docs/api_reference/preamble.js.html

var wasmBinary;
if (Module['wasmBinary']) wasmBinary = Module['wasmBinary'];legacyModuleProp('wasmBinary', 'wasmBinary');
var noExitRuntime = Module['noExitRuntime'] || true;legacyModuleProp('noExitRuntime', 'noExitRuntime');

if (typeof WebAssembly != 'object') {
  abort('no native wasm support detected');
}

// Wasm globals

var wasmMemory;
var wasmExports;

//========================================
// Runtime essentials
//========================================

// whether we are quitting the application. no code should run after this.
// set in exit() and abort()
var ABORT = false;

// set by exit() and abort().  Passed to 'onExit' handler.
// NOTE: This is also used as the process return code code in shell environments
// but only when noExitRuntime is false.
var EXITSTATUS;

/** @type {function(*, string=)} */
function assert(condition, text) {
  if (!condition) {
    abort('Assertion failed' + (text ? ': ' + text : ''));
  }
}

// We used to include malloc/free by default in the past. Show a helpful error in
// builds with assertions.

// Memory management

var HEAP,
/** @type {!Int8Array} */
  HEAP8,
/** @type {!Uint8Array} */
  HEAPU8,
/** @type {!Int16Array} */
  HEAP16,
/** @type {!Uint16Array} */
  HEAPU16,
/** @type {!Int32Array} */
  HEAP32,
/** @type {!Uint32Array} */
  HEAPU32,
/** @type {!Float32Array} */
  HEAPF32,
/** @type {!Float64Array} */
  HEAPF64;

function updateMemoryViews() {
  var b = wasmMemory.buffer;
  Module['HEAP8'] = HEAP8 = new Int8Array(b);
  Module['HEAP16'] = HEAP16 = new Int16Array(b);
  Module['HEAP32'] = HEAP32 = new Int32Array(b);
  Module['HEAPU8'] = HEAPU8 = new Uint8Array(b);
  Module['HEAPU16'] = HEAPU16 = new Uint16Array(b);
  Module['HEAPU32'] = HEAPU32 = new Uint32Array(b);
  Module['HEAPF32'] = HEAPF32 = new Float32Array(b);
  Module['HEAPF64'] = HEAPF64 = new Float64Array(b);
}

assert(!Module['STACK_SIZE'], 'STACK_SIZE can no longer be set at runtime.  Use -sSTACK_SIZE at link time')

assert(typeof Int32Array != 'undefined' && typeof Float64Array !== 'undefined' && Int32Array.prototype.subarray != undefined && Int32Array.prototype.set != undefined,
       'JS engine does not provide full typed array support');

// If memory is defined in wasm, the user can't provide it, or set INITIAL_MEMORY
assert(!Module['wasmMemory'], 'Use of `wasmMemory` detected.  Use -sIMPORTED_MEMORY to define wasmMemory externally');
assert(!Module['INITIAL_MEMORY'], 'Detected runtime INITIAL_MEMORY setting.  Use -sIMPORTED_MEMORY to define wasmMemory dynamically');

// include: runtime_init_table.js
// In regular non-RELOCATABLE mode the table is exported
// from the wasm module and this will be assigned once
// the exports are available.
var wasmTable;
// end include: runtime_init_table.js
// include: runtime_stack_check.js
// Initializes the stack cookie. Called at the startup of main and at the startup of each thread in pthreads mode.
function writeStackCookie() {
  var max = _emscripten_stack_get_end();
  assert((max & 3) == 0);
  // If the stack ends at address zero we write our cookies 4 bytes into the
  // stack.  This prevents interference with SAFE_HEAP and ASAN which also
  // monitor writes to address zero.
  if (max == 0) {
    max += 4;
  }
  // The stack grow downwards towards _emscripten_stack_get_end.
  // We write cookies to the final two words in the stack and detect if they are
  // ever overwritten.
  HEAPU32[((max)>>2)] = 0x02135467;
  HEAPU32[(((max)+(4))>>2)] = 0x89BACDFE;
  // Also test the global address 0 for integrity.
  HEAPU32[((0)>>2)] = 1668509029;
}

function checkStackCookie() {
  if (ABORT) return;
  var max = _emscripten_stack_get_end();
  // See writeStackCookie().
  if (max == 0) {
    max += 4;
  }
  var cookie1 = HEAPU32[((max)>>2)];
  var cookie2 = HEAPU32[(((max)+(4))>>2)];
  if (cookie1 != 0x02135467 || cookie2 != 0x89BACDFE) {
    abort(`Stack overflow! Stack cookie has been overwritten at ${ptrToString(max)}, expected hex dwords 0x89BACDFE and 0x2135467, but received ${ptrToString(cookie2)} ${ptrToString(cookie1)}`);
  }
  // Also test the global address 0 for integrity.
  if (HEAPU32[((0)>>2)] != 0x63736d65 /* 'emsc' */) {
    abort('Runtime error: The application has corrupted its heap memory area (address zero)!');
  }
}
// end include: runtime_stack_check.js
// include: runtime_assertions.js
// Endianness check
(function() {
  var h16 = new Int16Array(1);
  var h8 = new Int8Array(h16.buffer);
  h16[0] = 0x6373;
  if (h8[0] !== 0x73 || h8[1] !== 0x63) throw 'Runtime error: expected the system to be little-endian! (Run with -sSUPPORT_BIG_ENDIAN to bypass)';
})();

// end include: runtime_assertions.js
var __ATPRERUN__  = []; // functions called before the runtime is initialized
var __ATINIT__    = []; // functions called during startup
var __ATMAIN__    = []; // functions called when main() is to be run
var __ATEXIT__    = []; // functions called during shutdown
var __ATPOSTRUN__ = []; // functions called after the main() is called

var runtimeInitialized = false;

var runtimeKeepaliveCounter = 0;

function keepRuntimeAlive() {
  return noExitRuntime || runtimeKeepaliveCounter > 0;
}

function preRun() {
  if (Module['preRun']) {
    if (typeof Module['preRun'] == 'function') Module['preRun'] = [Module['preRun']];
    while (Module['preRun'].length) {
      addOnPreRun(Module['preRun'].shift());
    }
  }
  callRuntimeCallbacks(__ATPRERUN__);
}

function initRuntime() {
  assert(!runtimeInitialized);
  runtimeInitialized = true;

  checkStackCookie();

  
if (!Module["noFSInit"] && !FS.init.initialized)
  FS.init();
FS.ignorePermissions = false;

TTY.init();
  callRuntimeCallbacks(__ATINIT__);
}

function preMain() {
  checkStackCookie();
  
  callRuntimeCallbacks(__ATMAIN__);
}

function postRun() {
  checkStackCookie();

  if (Module['postRun']) {
    if (typeof Module['postRun'] == 'function') Module['postRun'] = [Module['postRun']];
    while (Module['postRun'].length) {
      addOnPostRun(Module['postRun'].shift());
    }
  }

  callRuntimeCallbacks(__ATPOSTRUN__);
}

function addOnPreRun(cb) {
  __ATPRERUN__.unshift(cb);
}

function addOnInit(cb) {
  __ATINIT__.unshift(cb);
}

function addOnPreMain(cb) {
  __ATMAIN__.unshift(cb);
}

function addOnExit(cb) {
}

function addOnPostRun(cb) {
  __ATPOSTRUN__.unshift(cb);
}

// include: runtime_math.js
// https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Math/imul

// https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Math/fround

// https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Math/clz32

// https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Math/trunc

assert(Math.imul, 'This browser does not support Math.imul(), build with LEGACY_VM_SUPPORT or POLYFILL_OLD_MATH_FUNCTIONS to add in a polyfill');
assert(Math.fround, 'This browser does not support Math.fround(), build with LEGACY_VM_SUPPORT or POLYFILL_OLD_MATH_FUNCTIONS to add in a polyfill');
assert(Math.clz32, 'This browser does not support Math.clz32(), build with LEGACY_VM_SUPPORT or POLYFILL_OLD_MATH_FUNCTIONS to add in a polyfill');
assert(Math.trunc, 'This browser does not support Math.trunc(), build with LEGACY_VM_SUPPORT or POLYFILL_OLD_MATH_FUNCTIONS to add in a polyfill');
// end include: runtime_math.js
// A counter of dependencies for calling run(). If we need to
// do asynchronous work before running, increment this and
// decrement it. Incrementing must happen in a place like
// Module.preRun (used by emcc to add file preloading).
// Note that you can add dependencies in preRun, even though
// it happens right before run - run will be postponed until
// the dependencies are met.
var runDependencies = 0;
var runDependencyWatcher = null;
var dependenciesFulfilled = null; // overridden to take different actions when all run dependencies are fulfilled
var runDependencyTracking = {};

function getUniqueRunDependency(id) {
  var orig = id;
  while (1) {
    if (!runDependencyTracking[id]) return id;
    id = orig + Math.random();
  }
}

function addRunDependency(id) {
  runDependencies++;

  if (Module['monitorRunDependencies']) {
    Module['monitorRunDependencies'](runDependencies);
  }

  if (id) {
    assert(!runDependencyTracking[id]);
    runDependencyTracking[id] = 1;
    if (runDependencyWatcher === null && typeof setInterval != 'undefined') {
      // Check for missing dependencies every few seconds
      runDependencyWatcher = setInterval(() => {
        if (ABORT) {
          clearInterval(runDependencyWatcher);
          runDependencyWatcher = null;
          return;
        }
        var shown = false;
        for (var dep in runDependencyTracking) {
          if (!shown) {
            shown = true;
            err('still waiting on run dependencies:');
          }
          err('dependency: ' + dep);
        }
        if (shown) {
          err('(end of list)');
        }
      }, 10000);
    }
  } else {
    err('warning: run dependency added without ID');
  }
}

function removeRunDependency(id) {
  runDependencies--;

  if (Module['monitorRunDependencies']) {
    Module['monitorRunDependencies'](runDependencies);
  }

  if (id) {
    assert(runDependencyTracking[id]);
    delete runDependencyTracking[id];
  } else {
    err('warning: run dependency removed without ID');
  }
  if (runDependencies == 0) {
    if (runDependencyWatcher !== null) {
      clearInterval(runDependencyWatcher);
      runDependencyWatcher = null;
    }
    if (dependenciesFulfilled) {
      var callback = dependenciesFulfilled;
      dependenciesFulfilled = null;
      callback(); // can add another dependenciesFulfilled
    }
  }
}

/** @param {string|number=} what */
function abort(what) {
  if (Module['onAbort']) {
    Module['onAbort'](what);
  }

  what = 'Aborted(' + what + ')';
  // TODO(sbc): Should we remove printing and leave it up to whoever
  // catches the exception?
  err(what);

  ABORT = true;
  EXITSTATUS = 1;

  // Use a wasm runtime error, because a JS error might be seen as a foreign
  // exception, which means we'd run destructors on it. We need the error to
  // simply make the program stop.
  // FIXME This approach does not work in Wasm EH because it currently does not assume
  // all RuntimeErrors are from traps; it decides whether a RuntimeError is from
  // a trap or not based on a hidden field within the object. So at the moment
  // we don't have a way of throwing a wasm trap from JS. TODO Make a JS API that
  // allows this in the wasm spec.

  // Suppress closure compiler warning here. Closure compiler's builtin extern
  // defintion for WebAssembly.RuntimeError claims it takes no arguments even
  // though it can.
  // TODO(https://github.com/google/closure-compiler/pull/3913): Remove if/when upstream closure gets fixed.
  /** @suppress {checkTypes} */
  var e = new WebAssembly.RuntimeError(what);

  // Throw the error whether or not MODULARIZE is set because abort is used
  // in code paths apart from instantiation where an exception is expected
  // to be thrown when abort is called.
  throw e;
}

// include: memoryprofiler.js
// end include: memoryprofiler.js
// include: URIUtils.js
// Prefix of data URIs emitted by SINGLE_FILE and related options.
var dataURIPrefix = 'data:application/octet-stream;base64,';

// Indicates whether filename is a base64 data URI.
function isDataURI(filename) {
  // Prefix of data URIs emitted by SINGLE_FILE and related options.
  return filename.startsWith(dataURIPrefix);
}

// Indicates whether filename is delivered via file protocol (as opposed to http/https)
function isFileURI(filename) {
  return filename.startsWith('file://');
}
// end include: URIUtils.js
function createExportWrapper(name) {
  return function() {
    assert(runtimeInitialized, `native function \`${name}\` called before runtime initialization`);
    var f = wasmExports[name];
    assert(f, `exported native function \`${name}\` not found`);
    return f.apply(null, arguments);
  };
}

// include: runtime_exceptions.js
// end include: runtime_exceptions.js
var wasmBinaryFile;
  wasmBinaryFile = 'Dungeons.wasm';
  if (!isDataURI(wasmBinaryFile)) {
    wasmBinaryFile = locateFile(wasmBinaryFile);
  }

function getBinarySync(file) {
  if (file == wasmBinaryFile && wasmBinary) {
    return new Uint8Array(wasmBinary);
  }
  if (readBinary) {
    return readBinary(file);
  }
  throw "both async and sync fetching of the wasm failed";
}

function getBinaryPromise(binaryFile) {
  // If we don't have the binary yet, try to load it asynchronously.
  // Fetch has some additional restrictions over XHR, like it can't be used on a file:// url.
  // See https://github.com/github/fetch/pull/92#issuecomment-140665932
  // Cordova or Electron apps are typically loaded from a file:// url.
  // So use fetch if it is available and the url is not a file, otherwise fall back to XHR.
  if (!wasmBinary
      && (ENVIRONMENT_IS_WEB || ENVIRONMENT_IS_WORKER)) {
    if (typeof fetch == 'function'
      && !isFileURI(binaryFile)
    ) {
      return fetch(binaryFile, { credentials: 'same-origin' }).then((response) => {
        if (!response['ok']) {
          throw "failed to load wasm binary file at '" + binaryFile + "'";
        }
        return response['arrayBuffer']();
      }).catch(() => getBinarySync(binaryFile));
    }
    else if (readAsync) {
      // fetch is not available or url is file => try XHR (readAsync uses XHR internally)
      return new Promise((resolve, reject) => {
        readAsync(binaryFile, (response) => resolve(new Uint8Array(/** @type{!ArrayBuffer} */(response))), reject)
      });
    }
  }

  // Otherwise, getBinarySync should be able to get it synchronously
  return Promise.resolve().then(() => getBinarySync(binaryFile));
}

function instantiateArrayBuffer(binaryFile, imports, receiver) {
  return getBinaryPromise(binaryFile).then((binary) => {
    return WebAssembly.instantiate(binary, imports);
  }).then((instance) => {
    return instance;
  }).then(receiver, (reason) => {
    err('failed to asynchronously prepare wasm: ' + reason);

    // Warn on some common problems.
    if (isFileURI(wasmBinaryFile)) {
      err('warning: Loading from a file URI (' + wasmBinaryFile + ') is not supported in most browsers. See https://emscripten.org/docs/getting_started/FAQ.html#how-do-i-run-a-local-webserver-for-testing-why-does-my-program-stall-in-downloading-or-preparing');
    }
    abort(reason);
  });
}

function instantiateAsync(binary, binaryFile, imports, callback) {
  if (!binary &&
      typeof WebAssembly.instantiateStreaming == 'function' &&
      !isDataURI(binaryFile) &&
      // Don't use streaming for file:// delivered objects in a webview, fetch them synchronously.
      !isFileURI(binaryFile) &&
      // Avoid instantiateStreaming() on Node.js environment for now, as while
      // Node.js v18.1.0 implements it, it does not have a full fetch()
      // implementation yet.
      //
      // Reference:
      //   https://github.com/emscripten-core/emscripten/pull/16917
      !ENVIRONMENT_IS_NODE &&
      typeof fetch == 'function') {
    return fetch(binaryFile, { credentials: 'same-origin' }).then((response) => {
      // Suppress closure warning here since the upstream definition for
      // instantiateStreaming only allows Promise<Repsponse> rather than
      // an actual Response.
      // TODO(https://github.com/google/closure-compiler/pull/3913): Remove if/when upstream closure is fixed.
      /** @suppress {checkTypes} */
      var result = WebAssembly.instantiateStreaming(response, imports);

      return result.then(
        callback,
        function(reason) {
          // We expect the most common failure cause to be a bad MIME type for the binary,
          // in which case falling back to ArrayBuffer instantiation should work.
          err('wasm streaming compile failed: ' + reason);
          err('falling back to ArrayBuffer instantiation');
          return instantiateArrayBuffer(binaryFile, imports, callback);
        });
    });
  }
  return instantiateArrayBuffer(binaryFile, imports, callback);
}

// Create the wasm instance.
// Receives the wasm imports, returns the exports.
function createWasm() {
  // prepare imports
  var info = {
    'env': wasmImports,
    'wasi_snapshot_preview1': wasmImports,
  };
  // Load the wasm module and create an instance of using native support in the JS engine.
  // handle a generated wasm instance, receiving its exports and
  // performing other necessary setup
  /** @param {WebAssembly.Module=} module*/
  function receiveInstance(instance, module) {
    var exports = instance.exports;

    wasmExports = exports;
    

    wasmMemory = wasmExports['memory'];
    
    assert(wasmMemory, "memory not found in wasm exports");
    // This assertion doesn't hold when emscripten is run in --post-link
    // mode.
    // TODO(sbc): Read INITIAL_MEMORY out of the wasm file in post-link mode.
    //assert(wasmMemory.buffer.byteLength === 16777216);
    updateMemoryViews();

    wasmTable = wasmExports['__indirect_function_table'];
    
    assert(wasmTable, "table not found in wasm exports");

    addOnInit(wasmExports['__wasm_call_ctors']);

    removeRunDependency('wasm-instantiate');
    return exports;
  }
  // wait for the pthread pool (if any)
  addRunDependency('wasm-instantiate');

  // Prefer streaming instantiation if available.
  // Async compilation can be confusing when an error on the page overwrites Module
  // (for example, if the order of elements is wrong, and the one defining Module is
  // later), so we save Module and check it later.
  var trueModule = Module;
  function receiveInstantiationResult(result) {
    // 'result' is a ResultObject object which has both the module and instance.
    // receiveInstance() will swap in the exports (to Module.asm) so they can be called
    assert(Module === trueModule, 'the Module object should not be replaced during async compilation - perhaps the order of HTML elements is wrong?');
    trueModule = null;
    // TODO: Due to Closure regression https://github.com/google/closure-compiler/issues/3193, the above line no longer optimizes out down to the following line.
    // When the regression is fixed, can restore the above PTHREADS-enabled path.
    receiveInstance(result['instance']);
  }

  // User shell pages can write their own Module.instantiateWasm = function(imports, successCallback) callback
  // to manually instantiate the Wasm module themselves. This allows pages to
  // run the instantiation parallel to any other async startup actions they are
  // performing.
  // Also pthreads and wasm workers initialize the wasm instance through this
  // path.
  if (Module['instantiateWasm']) {

    try {
      return Module['instantiateWasm'](info, receiveInstance);
    } catch(e) {
      err('Module.instantiateWasm callback failed with error: ' + e);
        return false;
    }
  }

  instantiateAsync(wasmBinary, wasmBinaryFile, info, receiveInstantiationResult);
  return {}; // no exports yet; we'll fill them in later
}

// Globals used by JS i64 conversions (see makeSetValue)
var tempDouble;
var tempI64;

// include: runtime_debug.js
function legacyModuleProp(prop, newName, incomming=true) {
  if (!Object.getOwnPropertyDescriptor(Module, prop)) {
    Object.defineProperty(Module, prop, {
      configurable: true,
      get() {
        let extra = incomming ? ' (the initial value can be provided on Module, but after startup the value is only looked for on a local variable of that name)' : '';
        abort(`\`Module.${prop}\` has been replaced by \`${newName}\`` + extra);

      }
    });
  }
}

function ignoredModuleProp(prop) {
  if (Object.getOwnPropertyDescriptor(Module, prop)) {
    abort(`\`Module.${prop}\` was supplied but \`${prop}\` not included in INCOMING_MODULE_JS_API`);
  }
}

// forcing the filesystem exports a few things by default
function isExportedByForceFilesystem(name) {
  return name === 'FS_createPath' ||
         name === 'FS_createDataFile' ||
         name === 'FS_createPreloadedFile' ||
         name === 'FS_unlink' ||
         name === 'addRunDependency' ||
         // The old FS has some functionality that WasmFS lacks.
         name === 'FS_createLazyFile' ||
         name === 'FS_createDevice' ||
         name === 'removeRunDependency';
}

function missingGlobal(sym, msg) {
  if (typeof globalThis !== 'undefined') {
    Object.defineProperty(globalThis, sym, {
      configurable: true,
      get() {
        warnOnce('`' + sym + '` is not longer defined by emscripten. ' + msg);
        return undefined;
      }
    });
  }
}

missingGlobal('buffer', 'Please use HEAP8.buffer or wasmMemory.buffer');

function missingLibrarySymbol(sym) {
  if (typeof globalThis !== 'undefined' && !Object.getOwnPropertyDescriptor(globalThis, sym)) {
    Object.defineProperty(globalThis, sym, {
      configurable: true,
      get() {
        // Can't `abort()` here because it would break code that does runtime
        // checks.  e.g. `if (typeof SDL === 'undefined')`.
        var msg = '`' + sym + '` is a library symbol and not included by default; add it to your library.js __deps or to DEFAULT_LIBRARY_FUNCS_TO_INCLUDE on the command line';
        // DEFAULT_LIBRARY_FUNCS_TO_INCLUDE requires the name as it appears in
        // library.js, which means $name for a JS name with no prefix, or name
        // for a JS name like _name.
        var librarySymbol = sym;
        if (!librarySymbol.startsWith('_')) {
          librarySymbol = '$' + sym;
        }
        msg += " (e.g. -sDEFAULT_LIBRARY_FUNCS_TO_INCLUDE='" + librarySymbol + "')";
        if (isExportedByForceFilesystem(sym)) {
          msg += '. Alternatively, forcing filesystem support (-sFORCE_FILESYSTEM) can export this for you';
        }
        warnOnce(msg);
        return undefined;
      }
    });
  }
  // Any symbol that is not included from the JS libary is also (by definition)
  // not exported on the Module object.
  unexportedRuntimeSymbol(sym);
}

function unexportedRuntimeSymbol(sym) {
  if (!Object.getOwnPropertyDescriptor(Module, sym)) {
    Object.defineProperty(Module, sym, {
      configurable: true,
      get() {
        var msg = "'" + sym + "' was not exported. add it to EXPORTED_RUNTIME_METHODS (see the Emscripten FAQ)";
        if (isExportedByForceFilesystem(sym)) {
          msg += '. Alternatively, forcing filesystem support (-sFORCE_FILESYSTEM) can export this for you';
        }
        abort(msg);
      }
    });
  }
}

// Used by XXXXX_DEBUG settings to output debug messages.
function dbg(text) {
  // TODO(sbc): Make this configurable somehow.  Its not always convenient for
  // logging to show up as warnings.
  console.warn.apply(console, arguments);
}
// end include: runtime_debug.js
// === Body ===

var ASM_CONSTS = {
  465572: ($0) => { var str = UTF8ToString($0) + '\n\n' + 'Abort/Retry/Ignore/AlwaysIgnore? [ariA] :'; var reply = window.prompt(str, "i"); if (reply === null) { reply = "i"; } return allocate(intArrayFromString(reply), 'i8', ALLOC_NORMAL); },  
 465797: () => { if (typeof(AudioContext) !== 'undefined') { return true; } else if (typeof(webkitAudioContext) !== 'undefined') { return true; } return false; },  
 465944: () => { if ((typeof(navigator.mediaDevices) !== 'undefined') && (typeof(navigator.mediaDevices.getUserMedia) !== 'undefined')) { return true; } else if (typeof(navigator.webkitGetUserMedia) !== 'undefined') { return true; } return false; },  
 466178: ($0) => { if(typeof(Module['SDL2']) === 'undefined') { Module['SDL2'] = {}; } var SDL2 = Module['SDL2']; if (!$0) { SDL2.audio = {}; } else { SDL2.capture = {}; } if (!SDL2.audioContext) { if (typeof(AudioContext) !== 'undefined') { SDL2.audioContext = new AudioContext(); } else if (typeof(webkitAudioContext) !== 'undefined') { SDL2.audioContext = new webkitAudioContext(); } if (SDL2.audioContext) { autoResumeAudioContext(SDL2.audioContext); } } return SDL2.audioContext === undefined ? -1 : 0; },  
 466671: () => { var SDL2 = Module['SDL2']; return SDL2.audioContext.sampleRate; },  
 466739: ($0, $1, $2, $3) => { var SDL2 = Module['SDL2']; var have_microphone = function(stream) { if (SDL2.capture.silenceTimer !== undefined) { clearTimeout(SDL2.capture.silenceTimer); SDL2.capture.silenceTimer = undefined; } SDL2.capture.mediaStreamNode = SDL2.audioContext.createMediaStreamSource(stream); SDL2.capture.scriptProcessorNode = SDL2.audioContext.createScriptProcessor($1, $0, 1); SDL2.capture.scriptProcessorNode.onaudioprocess = function(audioProcessingEvent) { if ((SDL2 === undefined) || (SDL2.capture === undefined)) { return; } audioProcessingEvent.outputBuffer.getChannelData(0).fill(0.0); SDL2.capture.currentCaptureBuffer = audioProcessingEvent.inputBuffer; dynCall('vi', $2, [$3]); }; SDL2.capture.mediaStreamNode.connect(SDL2.capture.scriptProcessorNode); SDL2.capture.scriptProcessorNode.connect(SDL2.audioContext.destination); SDL2.capture.stream = stream; }; var no_microphone = function(error) { }; SDL2.capture.silenceBuffer = SDL2.audioContext.createBuffer($0, $1, SDL2.audioContext.sampleRate); SDL2.capture.silenceBuffer.getChannelData(0).fill(0.0); var silence_callback = function() { SDL2.capture.currentCaptureBuffer = SDL2.capture.silenceBuffer; dynCall('vi', $2, [$3]); }; SDL2.capture.silenceTimer = setTimeout(silence_callback, ($1 / SDL2.audioContext.sampleRate) * 1000); if ((navigator.mediaDevices !== undefined) && (navigator.mediaDevices.getUserMedia !== undefined)) { navigator.mediaDevices.getUserMedia({ audio: true, video: false }).then(have_microphone).catch(no_microphone); } else if (navigator.webkitGetUserMedia !== undefined) { navigator.webkitGetUserMedia({ audio: true, video: false }, have_microphone, no_microphone); } },  
 468391: ($0, $1, $2, $3) => { var SDL2 = Module['SDL2']; SDL2.audio.scriptProcessorNode = SDL2.audioContext['createScriptProcessor']($1, 0, $0); SDL2.audio.scriptProcessorNode['onaudioprocess'] = function (e) { if ((SDL2 === undefined) || (SDL2.audio === undefined)) { return; } SDL2.audio.currentOutputBuffer = e['outputBuffer']; dynCall('vi', $2, [$3]); }; SDL2.audio.scriptProcessorNode['connect'](SDL2.audioContext['destination']); },  
 468801: ($0, $1) => { var SDL2 = Module['SDL2']; var numChannels = SDL2.capture.currentCaptureBuffer.numberOfChannels; for (var c = 0; c < numChannels; ++c) { var channelData = SDL2.capture.currentCaptureBuffer.getChannelData(c); if (channelData.length != $1) { throw 'Web Audio capture buffer length mismatch! Destination size: ' + channelData.length + ' samples vs expected ' + $1 + ' samples!'; } if (numChannels == 1) { for (var j = 0; j < $1; ++j) { setValue($0 + (j * 4), channelData[j], 'float'); } } else { for (var j = 0; j < $1; ++j) { setValue($0 + (((j * numChannels) + c) * 4), channelData[j], 'float'); } } } },  
 469406: ($0, $1) => { var SDL2 = Module['SDL2']; var numChannels = SDL2.audio.currentOutputBuffer['numberOfChannels']; for (var c = 0; c < numChannels; ++c) { var channelData = SDL2.audio.currentOutputBuffer['getChannelData'](c); if (channelData.length != $1) { throw 'Web Audio output buffer length mismatch! Destination size: ' + channelData.length + ' samples vs expected ' + $1 + ' samples!'; } for (var j = 0; j < $1; ++j) { channelData[j] = HEAPF32[$0 + ((j*numChannels + c) << 2) >> 2]; } } },  
 469886: ($0) => { var SDL2 = Module['SDL2']; if ($0) { if (SDL2.capture.silenceTimer !== undefined) { clearTimeout(SDL2.capture.silenceTimer); } if (SDL2.capture.stream !== undefined) { var tracks = SDL2.capture.stream.getAudioTracks(); for (var i = 0; i < tracks.length; i++) { SDL2.capture.stream.removeTrack(tracks[i]); } SDL2.capture.stream = undefined; } if (SDL2.capture.scriptProcessorNode !== undefined) { SDL2.capture.scriptProcessorNode.onaudioprocess = function(audioProcessingEvent) {}; SDL2.capture.scriptProcessorNode.disconnect(); SDL2.capture.scriptProcessorNode = undefined; } if (SDL2.capture.mediaStreamNode !== undefined) { SDL2.capture.mediaStreamNode.disconnect(); SDL2.capture.mediaStreamNode = undefined; } if (SDL2.capture.silenceBuffer !== undefined) { SDL2.capture.silenceBuffer = undefined } SDL2.capture = undefined; } else { if (SDL2.audio.scriptProcessorNode != undefined) { SDL2.audio.scriptProcessorNode.disconnect(); SDL2.audio.scriptProcessorNode = undefined; } SDL2.audio = undefined; } if ((SDL2.audioContext !== undefined) && (SDL2.audio === undefined) && (SDL2.capture === undefined)) { SDL2.audioContext.close(); SDL2.audioContext = undefined; } },  
 471058: ($0, $1, $2) => { var w = $0; var h = $1; var pixels = $2; if (!Module['SDL2']) Module['SDL2'] = {}; var SDL2 = Module['SDL2']; if (SDL2.ctxCanvas !== Module['canvas']) { SDL2.ctx = Module['createContext'](Module['canvas'], false, true); SDL2.ctxCanvas = Module['canvas']; } if (SDL2.w !== w || SDL2.h !== h || SDL2.imageCtx !== SDL2.ctx) { SDL2.image = SDL2.ctx.createImageData(w, h); SDL2.w = w; SDL2.h = h; SDL2.imageCtx = SDL2.ctx; } var data = SDL2.image.data; var src = pixels >> 2; var dst = 0; var num; if (typeof CanvasPixelArray !== 'undefined' && data instanceof CanvasPixelArray) { num = data.length; while (dst < num) { var val = HEAP32[src]; data[dst ] = val & 0xff; data[dst+1] = (val >> 8) & 0xff; data[dst+2] = (val >> 16) & 0xff; data[dst+3] = 0xff; src++; dst += 4; } } else { if (SDL2.data32Data !== data) { SDL2.data32 = new Int32Array(data.buffer); SDL2.data8 = new Uint8Array(data.buffer); SDL2.data32Data = data; } var data32 = SDL2.data32; num = data32.length; data32.set(HEAP32.subarray(src, src + num)); var data8 = SDL2.data8; var i = 3; var j = i + 4*num; if (num % 8 == 0) { while (i < j) { data8[i] = 0xff; i = i + 4 | 0; data8[i] = 0xff; i = i + 4 | 0; data8[i] = 0xff; i = i + 4 | 0; data8[i] = 0xff; i = i + 4 | 0; data8[i] = 0xff; i = i + 4 | 0; data8[i] = 0xff; i = i + 4 | 0; data8[i] = 0xff; i = i + 4 | 0; data8[i] = 0xff; i = i + 4 | 0; } } else { while (i < j) { data8[i] = 0xff; i = i + 4 | 0; } } } SDL2.ctx.putImageData(SDL2.image, 0, 0); },  
 472527: ($0, $1, $2, $3, $4) => { var w = $0; var h = $1; var hot_x = $2; var hot_y = $3; var pixels = $4; var canvas = document.createElement("canvas"); canvas.width = w; canvas.height = h; var ctx = canvas.getContext("2d"); var image = ctx.createImageData(w, h); var data = image.data; var src = pixels >> 2; var dst = 0; var num; if (typeof CanvasPixelArray !== 'undefined' && data instanceof CanvasPixelArray) { num = data.length; while (dst < num) { var val = HEAP32[src]; data[dst ] = val & 0xff; data[dst+1] = (val >> 8) & 0xff; data[dst+2] = (val >> 16) & 0xff; data[dst+3] = (val >> 24) & 0xff; src++; dst += 4; } } else { var data32 = new Int32Array(data.buffer); num = data32.length; data32.set(HEAP32.subarray(src, src + num)); } ctx.putImageData(image, 0, 0); var url = hot_x === 0 && hot_y === 0 ? "url(" + canvas.toDataURL() + "), auto" : "url(" + canvas.toDataURL() + ") " + hot_x + " " + hot_y + ", auto"; var urlBuf = _malloc(url.length + 1); stringToUTF8(url, urlBuf, url.length + 1); return urlBuf; },  
 473516: ($0) => { if (Module['canvas']) { Module['canvas'].style['cursor'] = UTF8ToString($0); } },  
 473599: () => { if (Module['canvas']) { Module['canvas'].style['cursor'] = 'none'; } },  
 473668: () => { return window.innerWidth; },  
 473698: () => { return window.innerHeight; }
};


// end include: preamble.js

  /** @constructor */
  function ExitStatus(status) {
      this.name = 'ExitStatus';
      this.message = `Program terminated with exit(${status})`;
      this.status = status;
    }

  var listenOnce = (object, event, func) => {
      object.addEventListener(event, func, { 'once': true });
    };
  /** @param {Object=} elements */
  var autoResumeAudioContext = (ctx, elements) => {
      if (!elements) {
        elements = [document, document.getElementById('canvas')];
      }
      ['keydown', 'mousedown', 'touchstart'].forEach((event) => {
        elements.forEach((element) => {
          if (element) {
            listenOnce(element, event, () => {
              if (ctx.state === 'suspended') ctx.resume();
            });
          }
        });
      });
    };

  var callRuntimeCallbacks = (callbacks) => {
      while (callbacks.length > 0) {
        // Pass the module as the first argument.
        callbacks.shift()(Module);
      }
    };

  var dynCallLegacy = (sig, ptr, args) => {
      assert(('dynCall_' + sig) in Module, `bad function pointer type - dynCall function not found for sig '${sig}'`);
      if (args && args.length) {
        // j (64-bit integer) must be passed in as two numbers [low 32, high 32].
        assert(args.length === sig.substring(1).replace(/j/g, '--').length);
      } else {
        assert(sig.length == 1);
      }
      var f = Module['dynCall_' + sig];
      return args && args.length ? f.apply(null, [ptr].concat(args)) : f.call(null, ptr);
    };
  
  var wasmTableMirror = [];
  var getWasmTableEntry = (funcPtr) => {
      var func = wasmTableMirror[funcPtr];
      if (!func) {
        if (funcPtr >= wasmTableMirror.length) wasmTableMirror.length = funcPtr + 1;
        wasmTableMirror[funcPtr] = func = wasmTable.get(funcPtr);
      }
      assert(wasmTable.get(funcPtr) == func, "JavaScript-side Wasm function table mirror is out of date!");
      return func;
    };
  
  /** @param {Object=} args */
  var dynCall = (sig, ptr, args) => {
      // Without WASM_BIGINT support we cannot directly call function with i64 as
      // part of thier signature, so we rely the dynCall functions generated by
      // wasm-emscripten-finalize
      if (sig.includes('j')) {
        return dynCallLegacy(sig, ptr, args);
      }
      assert(getWasmTableEntry(ptr), `missing table entry in dynCall: ${ptr}`);
      var rtn = getWasmTableEntry(ptr).apply(null, args);
      return rtn;
  
    };

  
    /**
     * @param {number} ptr
     * @param {string} type
     */
  function getValue(ptr, type = 'i8') {
    if (type.endsWith('*')) type = '*';
    switch (type) {
      case 'i1': return HEAP8[((ptr)>>0)];
      case 'i8': return HEAP8[((ptr)>>0)];
      case 'i16': return HEAP16[((ptr)>>1)];
      case 'i32': return HEAP32[((ptr)>>2)];
      case 'i64': abort('to do getValue(i64) use WASM_BIGINT');
      case 'float': return HEAPF32[((ptr)>>2)];
      case 'double': return HEAPF64[((ptr)>>3)];
      case '*': return HEAPU32[((ptr)>>2)];
      default: abort(`invalid type for getValue: ${type}`);
    }
  }

  var ptrToString = (ptr) => {
      assert(typeof ptr === 'number');
      // With CAN_ADDRESS_2GB or MEMORY64, pointers are already unsigned.
      ptr >>>= 0;
      return '0x' + ptr.toString(16).padStart(8, '0');
    };

  
    /**
     * @param {number} ptr
     * @param {number} value
     * @param {string} type
     */
  function setValue(ptr, value, type = 'i8') {
    if (type.endsWith('*')) type = '*';
    switch (type) {
      case 'i1': HEAP8[((ptr)>>0)] = value; break;
      case 'i8': HEAP8[((ptr)>>0)] = value; break;
      case 'i16': HEAP16[((ptr)>>1)] = value; break;
      case 'i32': HEAP32[((ptr)>>2)] = value; break;
      case 'i64': abort('to do setValue(i64) use WASM_BIGINT');
      case 'float': HEAPF32[((ptr)>>2)] = value; break;
      case 'double': HEAPF64[((ptr)>>3)] = value; break;
      case '*': HEAPU32[((ptr)>>2)] = value; break;
      default: abort(`invalid type for setValue: ${type}`);
    }
  }

  var warnOnce = (text) => {
      if (!warnOnce.shown) warnOnce.shown = {};
      if (!warnOnce.shown[text]) {
        warnOnce.shown[text] = 1;
        if (ENVIRONMENT_IS_NODE) text = 'warning: ' + text;
        err(text);
      }
    };

  /** @constructor */
  function ExceptionInfo(excPtr) {
      this.excPtr = excPtr;
      this.ptr = excPtr - 24;
  
      this.set_type = function(type) {
        HEAPU32[(((this.ptr)+(4))>>2)] = type;
      };
  
      this.get_type = function() {
        return HEAPU32[(((this.ptr)+(4))>>2)];
      };
  
      this.set_destructor = function(destructor) {
        HEAPU32[(((this.ptr)+(8))>>2)] = destructor;
      };
  
      this.get_destructor = function() {
        return HEAPU32[(((this.ptr)+(8))>>2)];
      };
  
      this.set_caught = function (caught) {
        caught = caught ? 1 : 0;
        HEAP8[(((this.ptr)+(12))>>0)] = caught;
      };
  
      this.get_caught = function () {
        return HEAP8[(((this.ptr)+(12))>>0)] != 0;
      };
  
      this.set_rethrown = function (rethrown) {
        rethrown = rethrown ? 1 : 0;
        HEAP8[(((this.ptr)+(13))>>0)] = rethrown;
      };
  
      this.get_rethrown = function () {
        return HEAP8[(((this.ptr)+(13))>>0)] != 0;
      };
  
      // Initialize native structure fields. Should be called once after allocated.
      this.init = function(type, destructor) {
        this.set_adjusted_ptr(0);
        this.set_type(type);
        this.set_destructor(destructor);
      }
  
      this.set_adjusted_ptr = function(adjustedPtr) {
        HEAPU32[(((this.ptr)+(16))>>2)] = adjustedPtr;
      };
  
      this.get_adjusted_ptr = function() {
        return HEAPU32[(((this.ptr)+(16))>>2)];
      };
  
      // Get pointer which is expected to be received by catch clause in C++ code. It may be adjusted
      // when the pointer is casted to some of the exception object base classes (e.g. when virtual
      // inheritance is used). When a pointer is thrown this method should return the thrown pointer
      // itself.
      this.get_exception_ptr = function() {
        // Work around a fastcomp bug, this code is still included for some reason in a build without
        // exceptions support.
        var isPointer = ___cxa_is_pointer_type(this.get_type());
        if (isPointer) {
          return HEAPU32[((this.excPtr)>>2)];
        }
        var adjusted = this.get_adjusted_ptr();
        if (adjusted !== 0) return adjusted;
        return this.excPtr;
      };
    }
  
  var exceptionLast = 0;
  
  var uncaughtExceptionCount = 0;
  function ___cxa_throw(ptr, type, destructor) {
      var info = new ExceptionInfo(ptr);
      // Initialize ExceptionInfo content after it was allocated in __cxa_allocate_exception.
      info.init(type, destructor);
      exceptionLast = ptr;
      uncaughtExceptionCount++;
      assert(false, 'Exception thrown, but exception catching is not enabled. Compile with -sNO_DISABLE_EXCEPTION_CATCHING or -sEXCEPTION_CATCHING_ALLOWED=[..] to catch.');
    }

  var setErrNo = (value) => {
      HEAP32[((___errno_location())>>2)] = value;
      return value;
    };
  
  var PATH = {
  isAbs:(path) => path.charAt(0) === '/',
  splitPath:(filename) => {
        var splitPathRe = /^(\/?|)([\s\S]*?)((?:\.{1,2}|[^\/]+?|)(\.[^.\/]*|))(?:[\/]*)$/;
        return splitPathRe.exec(filename).slice(1);
      },
  normalizeArray:(parts, allowAboveRoot) => {
        // if the path tries to go above the root, `up` ends up > 0
        var up = 0;
        for (var i = parts.length - 1; i >= 0; i--) {
          var last = parts[i];
          if (last === '.') {
            parts.splice(i, 1);
          } else if (last === '..') {
            parts.splice(i, 1);
            up++;
          } else if (up) {
            parts.splice(i, 1);
            up--;
          }
        }
        // if the path is allowed to go above the root, restore leading ..s
        if (allowAboveRoot) {
          for (; up; up--) {
            parts.unshift('..');
          }
        }
        return parts;
      },
  normalize:(path) => {
        var isAbsolute = PATH.isAbs(path),
            trailingSlash = path.substr(-1) === '/';
        // Normalize the path
        path = PATH.normalizeArray(path.split('/').filter((p) => !!p), !isAbsolute).join('/');
        if (!path && !isAbsolute) {
          path = '.';
        }
        if (path && trailingSlash) {
          path += '/';
        }
        return (isAbsolute ? '/' : '') + path;
      },
  dirname:(path) => {
        var result = PATH.splitPath(path),
            root = result[0],
            dir = result[1];
        if (!root && !dir) {
          // No dirname whatsoever
          return '.';
        }
        if (dir) {
          // It has a dirname, strip trailing slash
          dir = dir.substr(0, dir.length - 1);
        }
        return root + dir;
      },
  basename:(path) => {
        // EMSCRIPTEN return '/'' for '/', not an empty string
        if (path === '/') return '/';
        path = PATH.normalize(path);
        path = path.replace(/\/$/, "");
        var lastSlash = path.lastIndexOf('/');
        if (lastSlash === -1) return path;
        return path.substr(lastSlash+1);
      },
  join:function() {
        var paths = Array.prototype.slice.call(arguments);
        return PATH.normalize(paths.join('/'));
      },
  join2:(l, r) => {
        return PATH.normalize(l + '/' + r);
      },
  };
  
  var initRandomFill = () => {
      if (typeof crypto == 'object' && typeof crypto['getRandomValues'] == 'function') {
        // for modern web browsers
        return (view) => crypto.getRandomValues(view);
      } else
      if (ENVIRONMENT_IS_NODE) {
        // for nodejs with or without crypto support included
        try {
          var crypto_module = require('crypto');
          var randomFillSync = crypto_module['randomFillSync'];
          if (randomFillSync) {
            // nodejs with LTS crypto support
            return (view) => crypto_module['randomFillSync'](view);
          }
          // very old nodejs with the original crypto API
          var randomBytes = crypto_module['randomBytes'];
          return (view) => (
            view.set(randomBytes(view.byteLength)),
            // Return the original view to match modern native implementations.
            view
          );
        } catch (e) {
          // nodejs doesn't have crypto support
        }
      }
      // we couldn't find a proper implementation, as Math.random() is not suitable for /dev/random, see emscripten-core/emscripten/pull/7096
      abort("no cryptographic support found for randomDevice. consider polyfilling it if you want to use something insecure like Math.random(), e.g. put this in a --pre-js: var crypto = { getRandomValues: (array) => { for (var i = 0; i < array.length; i++) array[i] = (Math.random()*256)|0 } };");
    };
  var randomFill = (view) => {
      // Lazily init on the first invocation.
      return (randomFill = initRandomFill())(view);
    };
  
  
  
  var PATH_FS = {
  resolve:function() {
        var resolvedPath = '',
          resolvedAbsolute = false;
        for (var i = arguments.length - 1; i >= -1 && !resolvedAbsolute; i--) {
          var path = (i >= 0) ? arguments[i] : FS.cwd();
          // Skip empty and invalid entries
          if (typeof path != 'string') {
            throw new TypeError('Arguments to path.resolve must be strings');
          } else if (!path) {
            return ''; // an invalid portion invalidates the whole thing
          }
          resolvedPath = path + '/' + resolvedPath;
          resolvedAbsolute = PATH.isAbs(path);
        }
        // At this point the path should be resolved to a full absolute path, but
        // handle relative paths to be safe (might happen when process.cwd() fails)
        resolvedPath = PATH.normalizeArray(resolvedPath.split('/').filter((p) => !!p), !resolvedAbsolute).join('/');
        return ((resolvedAbsolute ? '/' : '') + resolvedPath) || '.';
      },
  relative:(from, to) => {
        from = PATH_FS.resolve(from).substr(1);
        to = PATH_FS.resolve(to).substr(1);
        function trim(arr) {
          var start = 0;
          for (; start < arr.length; start++) {
            if (arr[start] !== '') break;
          }
          var end = arr.length - 1;
          for (; end >= 0; end--) {
            if (arr[end] !== '') break;
          }
          if (start > end) return [];
          return arr.slice(start, end - start + 1);
        }
        var fromParts = trim(from.split('/'));
        var toParts = trim(to.split('/'));
        var length = Math.min(fromParts.length, toParts.length);
        var samePartsLength = length;
        for (var i = 0; i < length; i++) {
          if (fromParts[i] !== toParts[i]) {
            samePartsLength = i;
            break;
          }
        }
        var outputParts = [];
        for (var i = samePartsLength; i < fromParts.length; i++) {
          outputParts.push('..');
        }
        outputParts = outputParts.concat(toParts.slice(samePartsLength));
        return outputParts.join('/');
      },
  };
  
  
  var UTF8Decoder = typeof TextDecoder != 'undefined' ? new TextDecoder('utf8') : undefined;
  
    /**
     * Given a pointer 'idx' to a null-terminated UTF8-encoded string in the given
     * array that contains uint8 values, returns a copy of that string as a
     * Javascript String object.
     * heapOrArray is either a regular array, or a JavaScript typed array view.
     * @param {number} idx
     * @param {number=} maxBytesToRead
     * @return {string}
     */
  var UTF8ArrayToString = (heapOrArray, idx, maxBytesToRead) => {
      var endIdx = idx + maxBytesToRead;
      var endPtr = idx;
      // TextDecoder needs to know the byte length in advance, it doesn't stop on
      // null terminator by itself.  Also, use the length info to avoid running tiny
      // strings through TextDecoder, since .subarray() allocates garbage.
      // (As a tiny code save trick, compare endPtr against endIdx using a negation,
      // so that undefined means Infinity)
      while (heapOrArray[endPtr] && !(endPtr >= endIdx)) ++endPtr;
  
      if (endPtr - idx > 16 && heapOrArray.buffer && UTF8Decoder) {
        return UTF8Decoder.decode(heapOrArray.subarray(idx, endPtr));
      }
      var str = '';
      // If building with TextDecoder, we have already computed the string length
      // above, so test loop end condition against that
      while (idx < endPtr) {
        // For UTF8 byte structure, see:
        // http://en.wikipedia.org/wiki/UTF-8#Description
        // https://www.ietf.org/rfc/rfc2279.txt
        // https://tools.ietf.org/html/rfc3629
        var u0 = heapOrArray[idx++];
        if (!(u0 & 0x80)) { str += String.fromCharCode(u0); continue; }
        var u1 = heapOrArray[idx++] & 63;
        if ((u0 & 0xE0) == 0xC0) { str += String.fromCharCode(((u0 & 31) << 6) | u1); continue; }
        var u2 = heapOrArray[idx++] & 63;
        if ((u0 & 0xF0) == 0xE0) {
          u0 = ((u0 & 15) << 12) | (u1 << 6) | u2;
        } else {
          if ((u0 & 0xF8) != 0xF0) warnOnce('Invalid UTF-8 leading byte ' + ptrToString(u0) + ' encountered when deserializing a UTF-8 string in wasm memory to a JS string!');
          u0 = ((u0 & 7) << 18) | (u1 << 12) | (u2 << 6) | (heapOrArray[idx++] & 63);
        }
  
        if (u0 < 0x10000) {
          str += String.fromCharCode(u0);
        } else {
          var ch = u0 - 0x10000;
          str += String.fromCharCode(0xD800 | (ch >> 10), 0xDC00 | (ch & 0x3FF));
        }
      }
      return str;
    };
  
  var FS_stdin_getChar_buffer = [];
  
  var lengthBytesUTF8 = (str) => {
      var len = 0;
      for (var i = 0; i < str.length; ++i) {
        // Gotcha: charCodeAt returns a 16-bit word that is a UTF-16 encoded code
        // unit, not a Unicode code point of the character! So decode
        // UTF16->UTF32->UTF8.
        // See http://unicode.org/faq/utf_bom.html#utf16-3
        var c = str.charCodeAt(i); // possibly a lead surrogate
        if (c <= 0x7F) {
          len++;
        } else if (c <= 0x7FF) {
          len += 2;
        } else if (c >= 0xD800 && c <= 0xDFFF) {
          len += 4; ++i;
        } else {
          len += 3;
        }
      }
      return len;
    };
  
  var stringToUTF8Array = (str, heap, outIdx, maxBytesToWrite) => {
      assert(typeof str === 'string');
      // Parameter maxBytesToWrite is not optional. Negative values, 0, null,
      // undefined and false each don't write out any bytes.
      if (!(maxBytesToWrite > 0))
        return 0;
  
      var startIdx = outIdx;
      var endIdx = outIdx + maxBytesToWrite - 1; // -1 for string null terminator.
      for (var i = 0; i < str.length; ++i) {
        // Gotcha: charCodeAt returns a 16-bit word that is a UTF-16 encoded code
        // unit, not a Unicode code point of the character! So decode
        // UTF16->UTF32->UTF8.
        // See http://unicode.org/faq/utf_bom.html#utf16-3
        // For UTF8 byte structure, see http://en.wikipedia.org/wiki/UTF-8#Description
        // and https://www.ietf.org/rfc/rfc2279.txt
        // and https://tools.ietf.org/html/rfc3629
        var u = str.charCodeAt(i); // possibly a lead surrogate
        if (u >= 0xD800 && u <= 0xDFFF) {
          var u1 = str.charCodeAt(++i);
          u = 0x10000 + ((u & 0x3FF) << 10) | (u1 & 0x3FF);
        }
        if (u <= 0x7F) {
          if (outIdx >= endIdx) break;
          heap[outIdx++] = u;
        } else if (u <= 0x7FF) {
          if (outIdx + 1 >= endIdx) break;
          heap[outIdx++] = 0xC0 | (u >> 6);
          heap[outIdx++] = 0x80 | (u & 63);
        } else if (u <= 0xFFFF) {
          if (outIdx + 2 >= endIdx) break;
          heap[outIdx++] = 0xE0 | (u >> 12);
          heap[outIdx++] = 0x80 | ((u >> 6) & 63);
          heap[outIdx++] = 0x80 | (u & 63);
        } else {
          if (outIdx + 3 >= endIdx) break;
          if (u > 0x10FFFF) warnOnce('Invalid Unicode code point ' + ptrToString(u) + ' encountered when serializing a JS string to a UTF-8 string in wasm memory! (Valid unicode code points should be in range 0-0x10FFFF).');
          heap[outIdx++] = 0xF0 | (u >> 18);
          heap[outIdx++] = 0x80 | ((u >> 12) & 63);
          heap[outIdx++] = 0x80 | ((u >> 6) & 63);
          heap[outIdx++] = 0x80 | (u & 63);
        }
      }
      // Null-terminate the pointer to the buffer.
      heap[outIdx] = 0;
      return outIdx - startIdx;
    };
  /** @type {function(string, boolean=, number=)} */
  function intArrayFromString(stringy, dontAddNull, length) {
    var len = length > 0 ? length : lengthBytesUTF8(stringy)+1;
    var u8array = new Array(len);
    var numBytesWritten = stringToUTF8Array(stringy, u8array, 0, u8array.length);
    if (dontAddNull) u8array.length = numBytesWritten;
    return u8array;
  }
  var FS_stdin_getChar = () => {
      if (!FS_stdin_getChar_buffer.length) {
        var result = null;
        if (ENVIRONMENT_IS_NODE) {
          // we will read data by chunks of BUFSIZE
          var BUFSIZE = 256;
          var buf = Buffer.alloc(BUFSIZE);
          var bytesRead = 0;
  
          // For some reason we must suppress a closure warning here, even though
          // fd definitely exists on process.stdin, and is even the proper way to
          // get the fd of stdin,
          // https://github.com/nodejs/help/issues/2136#issuecomment-523649904
          // This started to happen after moving this logic out of library_tty.js,
          // so it is related to the surrounding code in some unclear manner.
          /** @suppress {missingProperties} */
          var fd = process.stdin.fd;
  
          try {
            bytesRead = fs.readSync(fd, buf, 0, BUFSIZE, -1);
          } catch(e) {
            // Cross-platform differences: on Windows, reading EOF throws an exception, but on other OSes,
            // reading EOF returns 0. Uniformize behavior by treating the EOF exception to return 0.
            if (e.toString().includes('EOF')) bytesRead = 0;
            else throw e;
          }
  
          if (bytesRead > 0) {
            result = buf.slice(0, bytesRead).toString('utf-8');
          } else {
            result = null;
          }
        } else
        if (typeof window != 'undefined' &&
          typeof window.prompt == 'function') {
          // Browser.
          result = window.prompt('Input: ');  // returns null on cancel
          if (result !== null) {
            result += '\n';
          }
        } else if (typeof readline == 'function') {
          // Command line.
          result = readline();
          if (result !== null) {
            result += '\n';
          }
        }
        if (!result) {
          return null;
        }
        FS_stdin_getChar_buffer = intArrayFromString(result, true);
      }
      return FS_stdin_getChar_buffer.shift();
    };
  var TTY = {
  ttys:[],
  init:function () {
        // https://github.com/emscripten-core/emscripten/pull/1555
        // if (ENVIRONMENT_IS_NODE) {
        //   // currently, FS.init does not distinguish if process.stdin is a file or TTY
        //   // device, it always assumes it's a TTY device. because of this, we're forcing
        //   // process.stdin to UTF8 encoding to at least make stdin reading compatible
        //   // with text files until FS.init can be refactored.
        //   process.stdin.setEncoding('utf8');
        // }
      },
  shutdown:function() {
        // https://github.com/emscripten-core/emscripten/pull/1555
        // if (ENVIRONMENT_IS_NODE) {
        //   // inolen: any idea as to why node -e 'process.stdin.read()' wouldn't exit immediately (with process.stdin being a tty)?
        //   // isaacs: because now it's reading from the stream, you've expressed interest in it, so that read() kicks off a _read() which creates a ReadReq operation
        //   // inolen: I thought read() in that case was a synchronous operation that just grabbed some amount of buffered data if it exists?
        //   // isaacs: it is. but it also triggers a _read() call, which calls readStart() on the handle
        //   // isaacs: do process.stdin.pause() and i'd think it'd probably close the pending call
        //   process.stdin.pause();
        // }
      },
  register:function(dev, ops) {
        TTY.ttys[dev] = { input: [], output: [], ops: ops };
        FS.registerDevice(dev, TTY.stream_ops);
      },
  stream_ops:{
  open:function(stream) {
          var tty = TTY.ttys[stream.node.rdev];
          if (!tty) {
            throw new FS.ErrnoError(43);
          }
          stream.tty = tty;
          stream.seekable = false;
        },
  close:function(stream) {
          // flush any pending line data
          stream.tty.ops.fsync(stream.tty);
        },
  fsync:function(stream) {
          stream.tty.ops.fsync(stream.tty);
        },
  read:function(stream, buffer, offset, length, pos /* ignored */) {
          if (!stream.tty || !stream.tty.ops.get_char) {
            throw new FS.ErrnoError(60);
          }
          var bytesRead = 0;
          for (var i = 0; i < length; i++) {
            var result;
            try {
              result = stream.tty.ops.get_char(stream.tty);
            } catch (e) {
              throw new FS.ErrnoError(29);
            }
            if (result === undefined && bytesRead === 0) {
              throw new FS.ErrnoError(6);
            }
            if (result === null || result === undefined) break;
            bytesRead++;
            buffer[offset+i] = result;
          }
          if (bytesRead) {
            stream.node.timestamp = Date.now();
          }
          return bytesRead;
        },
  write:function(stream, buffer, offset, length, pos) {
          if (!stream.tty || !stream.tty.ops.put_char) {
            throw new FS.ErrnoError(60);
          }
          try {
            for (var i = 0; i < length; i++) {
              stream.tty.ops.put_char(stream.tty, buffer[offset+i]);
            }
          } catch (e) {
            throw new FS.ErrnoError(29);
          }
          if (length) {
            stream.node.timestamp = Date.now();
          }
          return i;
        },
  },
  default_tty_ops:{
  get_char:function(tty) {
          return FS_stdin_getChar();
        },
  put_char:function(tty, val) {
          if (val === null || val === 10) {
            out(UTF8ArrayToString(tty.output, 0));
            tty.output = [];
          } else {
            if (val != 0) tty.output.push(val); // val == 0 would cut text output off in the middle.
          }
        },
  fsync:function(tty) {
          if (tty.output && tty.output.length > 0) {
            out(UTF8ArrayToString(tty.output, 0));
            tty.output = [];
          }
        },
  ioctl_tcgets:function(tty) {
          // typical setting
          return {
            c_iflag: 25856,
            c_oflag: 5,
            c_cflag: 191,
            c_lflag: 35387,
            c_cc: [
              0x03, 0x1c, 0x7f, 0x15, 0x04, 0x00, 0x01, 0x00, 0x11, 0x13, 0x1a, 0x00,
              0x12, 0x0f, 0x17, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            ]
          };
        },
  ioctl_tcsets:function(tty, optional_actions, data) {
          // currently just ignore
          return 0;
        },
  ioctl_tiocgwinsz:function(tty) {
          return [24, 80];
        },
  },
  default_tty1_ops:{
  put_char:function(tty, val) {
          if (val === null || val === 10) {
            err(UTF8ArrayToString(tty.output, 0));
            tty.output = [];
          } else {
            if (val != 0) tty.output.push(val);
          }
        },
  fsync:function(tty) {
          if (tty.output && tty.output.length > 0) {
            err(UTF8ArrayToString(tty.output, 0));
            tty.output = [];
          }
        },
  },
  };
  
  
  var zeroMemory = (address, size) => {
      HEAPU8.fill(0, address, address + size);
      return address;
    };
  
  var alignMemory = (size, alignment) => {
      assert(alignment, "alignment argument is required");
      return Math.ceil(size / alignment) * alignment;
    };
  var mmapAlloc = (size) => {
      size = alignMemory(size, 65536);
      var ptr = _emscripten_builtin_memalign(65536, size);
      if (!ptr) return 0;
      return zeroMemory(ptr, size);
    };
  var MEMFS = {
  ops_table:null,
  mount(mount) {
        return MEMFS.createNode(null, '/', 16384 | 511 /* 0777 */, 0);
      },
  createNode(parent, name, mode, dev) {
        if (FS.isBlkdev(mode) || FS.isFIFO(mode)) {
          // no supported
          throw new FS.ErrnoError(63);
        }
        if (!MEMFS.ops_table) {
          MEMFS.ops_table = {
            dir: {
              node: {
                getattr: MEMFS.node_ops.getattr,
                setattr: MEMFS.node_ops.setattr,
                lookup: MEMFS.node_ops.lookup,
                mknod: MEMFS.node_ops.mknod,
                rename: MEMFS.node_ops.rename,
                unlink: MEMFS.node_ops.unlink,
                rmdir: MEMFS.node_ops.rmdir,
                readdir: MEMFS.node_ops.readdir,
                symlink: MEMFS.node_ops.symlink
              },
              stream: {
                llseek: MEMFS.stream_ops.llseek
              }
            },
            file: {
              node: {
                getattr: MEMFS.node_ops.getattr,
                setattr: MEMFS.node_ops.setattr
              },
              stream: {
                llseek: MEMFS.stream_ops.llseek,
                read: MEMFS.stream_ops.read,
                write: MEMFS.stream_ops.write,
                allocate: MEMFS.stream_ops.allocate,
                mmap: MEMFS.stream_ops.mmap,
                msync: MEMFS.stream_ops.msync
              }
            },
            link: {
              node: {
                getattr: MEMFS.node_ops.getattr,
                setattr: MEMFS.node_ops.setattr,
                readlink: MEMFS.node_ops.readlink
              },
              stream: {}
            },
            chrdev: {
              node: {
                getattr: MEMFS.node_ops.getattr,
                setattr: MEMFS.node_ops.setattr
              },
              stream: FS.chrdev_stream_ops
            }
          };
        }
        var node = FS.createNode(parent, name, mode, dev);
        if (FS.isDir(node.mode)) {
          node.node_ops = MEMFS.ops_table.dir.node;
          node.stream_ops = MEMFS.ops_table.dir.stream;
          node.contents = {};
        } else if (FS.isFile(node.mode)) {
          node.node_ops = MEMFS.ops_table.file.node;
          node.stream_ops = MEMFS.ops_table.file.stream;
          node.usedBytes = 0; // The actual number of bytes used in the typed array, as opposed to contents.length which gives the whole capacity.
          // When the byte data of the file is populated, this will point to either a typed array, or a normal JS array. Typed arrays are preferred
          // for performance, and used by default. However, typed arrays are not resizable like normal JS arrays are, so there is a small disk size
          // penalty involved for appending file writes that continuously grow a file similar to std::vector capacity vs used -scheme.
          node.contents = null; 
        } else if (FS.isLink(node.mode)) {
          node.node_ops = MEMFS.ops_table.link.node;
          node.stream_ops = MEMFS.ops_table.link.stream;
        } else if (FS.isChrdev(node.mode)) {
          node.node_ops = MEMFS.ops_table.chrdev.node;
          node.stream_ops = MEMFS.ops_table.chrdev.stream;
        }
        node.timestamp = Date.now();
        // add the new node to the parent
        if (parent) {
          parent.contents[name] = node;
          parent.timestamp = node.timestamp;
        }
        return node;
      },
  getFileDataAsTypedArray(node) {
        if (!node.contents) return new Uint8Array(0);
        if (node.contents.subarray) return node.contents.subarray(0, node.usedBytes); // Make sure to not return excess unused bytes.
        return new Uint8Array(node.contents);
      },
  expandFileStorage(node, newCapacity) {
        var prevCapacity = node.contents ? node.contents.length : 0;
        if (prevCapacity >= newCapacity) return; // No need to expand, the storage was already large enough.
        // Don't expand strictly to the given requested limit if it's only a very small increase, but instead geometrically grow capacity.
        // For small filesizes (<1MB), perform size*2 geometric increase, but for large sizes, do a much more conservative size*1.125 increase to
        // avoid overshooting the allocation cap by a very large margin.
        var CAPACITY_DOUBLING_MAX = 1024 * 1024;
        newCapacity = Math.max(newCapacity, (prevCapacity * (prevCapacity < CAPACITY_DOUBLING_MAX ? 2.0 : 1.125)) >>> 0);
        if (prevCapacity != 0) newCapacity = Math.max(newCapacity, 256); // At minimum allocate 256b for each file when expanding.
        var oldContents = node.contents;
        node.contents = new Uint8Array(newCapacity); // Allocate new storage.
        if (node.usedBytes > 0) node.contents.set(oldContents.subarray(0, node.usedBytes), 0); // Copy old data over to the new storage.
      },
  resizeFileStorage(node, newSize) {
        if (node.usedBytes == newSize) return;
        if (newSize == 0) {
          node.contents = null; // Fully decommit when requesting a resize to zero.
          node.usedBytes = 0;
        } else {
          var oldContents = node.contents;
          node.contents = new Uint8Array(newSize); // Allocate new storage.
          if (oldContents) {
            node.contents.set(oldContents.subarray(0, Math.min(newSize, node.usedBytes))); // Copy old data over to the new storage.
          }
          node.usedBytes = newSize;
        }
      },
  node_ops:{
  getattr(node) {
          var attr = {};
          // device numbers reuse inode numbers.
          attr.dev = FS.isChrdev(node.mode) ? node.id : 1;
          attr.ino = node.id;
          attr.mode = node.mode;
          attr.nlink = 1;
          attr.uid = 0;
          attr.gid = 0;
          attr.rdev = node.rdev;
          if (FS.isDir(node.mode)) {
            attr.size = 4096;
          } else if (FS.isFile(node.mode)) {
            attr.size = node.usedBytes;
          } else if (FS.isLink(node.mode)) {
            attr.size = node.link.length;
          } else {
            attr.size = 0;
          }
          attr.atime = new Date(node.timestamp);
          attr.mtime = new Date(node.timestamp);
          attr.ctime = new Date(node.timestamp);
          // NOTE: In our implementation, st_blocks = Math.ceil(st_size/st_blksize),
          //       but this is not required by the standard.
          attr.blksize = 4096;
          attr.blocks = Math.ceil(attr.size / attr.blksize);
          return attr;
        },
  setattr(node, attr) {
          if (attr.mode !== undefined) {
            node.mode = attr.mode;
          }
          if (attr.timestamp !== undefined) {
            node.timestamp = attr.timestamp;
          }
          if (attr.size !== undefined) {
            MEMFS.resizeFileStorage(node, attr.size);
          }
        },
  lookup(parent, name) {
          throw FS.genericErrors[44];
        },
  mknod(parent, name, mode, dev) {
          return MEMFS.createNode(parent, name, mode, dev);
        },
  rename(old_node, new_dir, new_name) {
          // if we're overwriting a directory at new_name, make sure it's empty.
          if (FS.isDir(old_node.mode)) {
            var new_node;
            try {
              new_node = FS.lookupNode(new_dir, new_name);
            } catch (e) {
            }
            if (new_node) {
              for (var i in new_node.contents) {
                throw new FS.ErrnoError(55);
              }
            }
          }
          // do the internal rewiring
          delete old_node.parent.contents[old_node.name];
          old_node.parent.timestamp = Date.now()
          old_node.name = new_name;
          new_dir.contents[new_name] = old_node;
          new_dir.timestamp = old_node.parent.timestamp;
          old_node.parent = new_dir;
        },
  unlink(parent, name) {
          delete parent.contents[name];
          parent.timestamp = Date.now();
        },
  rmdir(parent, name) {
          var node = FS.lookupNode(parent, name);
          for (var i in node.contents) {
            throw new FS.ErrnoError(55);
          }
          delete parent.contents[name];
          parent.timestamp = Date.now();
        },
  readdir(node) {
          var entries = ['.', '..'];
          for (var key in node.contents) {
            if (!node.contents.hasOwnProperty(key)) {
              continue;
            }
            entries.push(key);
          }
          return entries;
        },
  symlink(parent, newname, oldpath) {
          var node = MEMFS.createNode(parent, newname, 511 /* 0777 */ | 40960, 0);
          node.link = oldpath;
          return node;
        },
  readlink(node) {
          if (!FS.isLink(node.mode)) {
            throw new FS.ErrnoError(28);
          }
          return node.link;
        },
  },
  stream_ops:{
  read(stream, buffer, offset, length, position) {
          var contents = stream.node.contents;
          if (position >= stream.node.usedBytes) return 0;
          var size = Math.min(stream.node.usedBytes - position, length);
          assert(size >= 0);
          if (size > 8 && contents.subarray) { // non-trivial, and typed array
            buffer.set(contents.subarray(position, position + size), offset);
          } else {
            for (var i = 0; i < size; i++) buffer[offset + i] = contents[position + i];
          }
          return size;
        },
  write(stream, buffer, offset, length, position, canOwn) {
          // The data buffer should be a typed array view
          assert(!(buffer instanceof ArrayBuffer));
          // If the buffer is located in main memory (HEAP), and if
          // memory can grow, we can't hold on to references of the
          // memory buffer, as they may get invalidated. That means we
          // need to do copy its contents.
          if (buffer.buffer === HEAP8.buffer) {
            canOwn = false;
          }
  
          if (!length) return 0;
          var node = stream.node;
          node.timestamp = Date.now();
  
          if (buffer.subarray && (!node.contents || node.contents.subarray)) { // This write is from a typed array to a typed array?
            if (canOwn) {
              assert(position === 0, 'canOwn must imply no weird position inside the file');
              node.contents = buffer.subarray(offset, offset + length);
              node.usedBytes = length;
              return length;
            } else if (node.usedBytes === 0 && position === 0) { // If this is a simple first write to an empty file, do a fast set since we don't need to care about old data.
              node.contents = buffer.slice(offset, offset + length);
              node.usedBytes = length;
              return length;
            } else if (position + length <= node.usedBytes) { // Writing to an already allocated and used subrange of the file?
              node.contents.set(buffer.subarray(offset, offset + length), position);
              return length;
            }
          }
  
          // Appending to an existing file and we need to reallocate, or source data did not come as a typed array.
          MEMFS.expandFileStorage(node, position+length);
          if (node.contents.subarray && buffer.subarray) {
            // Use typed array write which is available.
            node.contents.set(buffer.subarray(offset, offset + length), position);
          } else {
            for (var i = 0; i < length; i++) {
             node.contents[position + i] = buffer[offset + i]; // Or fall back to manual write if not.
            }
          }
          node.usedBytes = Math.max(node.usedBytes, position + length);
          return length;
        },
  llseek(stream, offset, whence) {
          var position = offset;
          if (whence === 1) {
            position += stream.position;
          } else if (whence === 2) {
            if (FS.isFile(stream.node.mode)) {
              position += stream.node.usedBytes;
            }
          }
          if (position < 0) {
            throw new FS.ErrnoError(28);
          }
          return position;
        },
  allocate(stream, offset, length) {
          MEMFS.expandFileStorage(stream.node, offset + length);
          stream.node.usedBytes = Math.max(stream.node.usedBytes, offset + length);
        },
  mmap(stream, length, position, prot, flags) {
          if (!FS.isFile(stream.node.mode)) {
            throw new FS.ErrnoError(43);
          }
          var ptr;
          var allocated;
          var contents = stream.node.contents;
          // Only make a new copy when MAP_PRIVATE is specified.
          if (!(flags & 2) && contents.buffer === HEAP8.buffer) {
            // We can't emulate MAP_SHARED when the file is not backed by the
            // buffer we're mapping to (e.g. the HEAP buffer).
            allocated = false;
            ptr = contents.byteOffset;
          } else {
            // Try to avoid unnecessary slices.
            if (position > 0 || position + length < contents.length) {
              if (contents.subarray) {
                contents = contents.subarray(position, position + length);
              } else {
                contents = Array.prototype.slice.call(contents, position, position + length);
              }
            }
            allocated = true;
            ptr = mmapAlloc(length);
            if (!ptr) {
              throw new FS.ErrnoError(48);
            }
            HEAP8.set(contents, ptr);
          }
          return { ptr, allocated };
        },
  msync(stream, buffer, offset, length, mmapFlags) {
          MEMFS.stream_ops.write(stream, buffer, 0, length, offset, false);
          // should we check if bytesWritten and length are the same?
          return 0;
        },
  },
  };
  
  /** @param {boolean=} noRunDep */
  var asyncLoad = (url, onload, onerror, noRunDep) => {
      var dep = !noRunDep ? getUniqueRunDependency(`al ${url}`) : '';
      readAsync(url, (arrayBuffer) => {
        assert(arrayBuffer, `Loading data file "${url}" failed (no arrayBuffer).`);
        onload(new Uint8Array(arrayBuffer));
        if (dep) removeRunDependency(dep);
      }, (event) => {
        if (onerror) {
          onerror();
        } else {
          throw `Loading data file "${url}" failed.`;
        }
      });
      if (dep) addRunDependency(dep);
    };
  
  
  var preloadPlugins = Module['preloadPlugins'] || [];
  function FS_handledByPreloadPlugin(byteArray, fullname, finish, onerror) {
      // Ensure plugins are ready.
      if (typeof Browser != 'undefined') Browser.init();
  
      var handled = false;
      preloadPlugins.forEach(function(plugin) {
        if (handled) return;
        if (plugin['canHandle'](fullname)) {
          plugin['handle'](byteArray, fullname, finish, onerror);
          handled = true;
        }
      });
      return handled;
    }
  function FS_createPreloadedFile(parent, name, url, canRead, canWrite, onload, onerror, dontCreateFile, canOwn, preFinish) {
      // TODO we should allow people to just pass in a complete filename instead
      // of parent and name being that we just join them anyways
      var fullname = name ? PATH_FS.resolve(PATH.join2(parent, name)) : parent;
      var dep = getUniqueRunDependency(`cp ${fullname}`); // might have several active requests for the same fullname
      function processData(byteArray) {
        function finish(byteArray) {
          if (preFinish) preFinish();
          if (!dontCreateFile) {
            FS.createDataFile(parent, name, byteArray, canRead, canWrite, canOwn);
          }
          if (onload) onload();
          removeRunDependency(dep);
        }
        if (FS_handledByPreloadPlugin(byteArray, fullname, finish, () => {
          if (onerror) onerror();
          removeRunDependency(dep);
        })) {
          return;
        }
        finish(byteArray);
      }
      addRunDependency(dep);
      if (typeof url == 'string') {
        asyncLoad(url, (byteArray) => processData(byteArray), onerror);
      } else {
        processData(url);
      }
    }
  
  function FS_modeStringToFlags(str) {
      var flagModes = {
        'r': 0,
        'r+': 2,
        'w': 512 | 64 | 1,
        'w+': 512 | 64 | 2,
        'a': 1024 | 64 | 1,
        'a+': 1024 | 64 | 2,
      };
      var flags = flagModes[str];
      if (typeof flags == 'undefined') {
        throw new Error(`Unknown file open mode: ${str}`);
      }
      return flags;
    }
  
  function FS_getMode(canRead, canWrite) {
      var mode = 0;
      if (canRead) mode |= 292 | 73;
      if (canWrite) mode |= 146;
      return mode;
    }
  
  
  
  
  var ERRNO_MESSAGES = {
  0:"Success",
  1:"Arg list too long",
  2:"Permission denied",
  3:"Address already in use",
  4:"Address not available",
  5:"Address family not supported by protocol family",
  6:"No more processes",
  7:"Socket already connected",
  8:"Bad file number",
  9:"Trying to read unreadable message",
  10:"Mount device busy",
  11:"Operation canceled",
  12:"No children",
  13:"Connection aborted",
  14:"Connection refused",
  15:"Connection reset by peer",
  16:"File locking deadlock error",
  17:"Destination address required",
  18:"Math arg out of domain of func",
  19:"Quota exceeded",
  20:"File exists",
  21:"Bad address",
  22:"File too large",
  23:"Host is unreachable",
  24:"Identifier removed",
  25:"Illegal byte sequence",
  26:"Connection already in progress",
  27:"Interrupted system call",
  28:"Invalid argument",
  29:"I/O error",
  30:"Socket is already connected",
  31:"Is a directory",
  32:"Too many symbolic links",
  33:"Too many open files",
  34:"Too many links",
  35:"Message too long",
  36:"Multihop attempted",
  37:"File or path name too long",
  38:"Network interface is not configured",
  39:"Connection reset by network",
  40:"Network is unreachable",
  41:"Too many open files in system",
  42:"No buffer space available",
  43:"No such device",
  44:"No such file or directory",
  45:"Exec format error",
  46:"No record locks available",
  47:"The link has been severed",
  48:"Not enough core",
  49:"No message of desired type",
  50:"Protocol not available",
  51:"No space left on device",
  52:"Function not implemented",
  53:"Socket is not connected",
  54:"Not a directory",
  55:"Directory not empty",
  56:"State not recoverable",
  57:"Socket operation on non-socket",
  59:"Not a typewriter",
  60:"No such device or address",
  61:"Value too large for defined data type",
  62:"Previous owner died",
  63:"Not super-user",
  64:"Broken pipe",
  65:"Protocol error",
  66:"Unknown protocol",
  67:"Protocol wrong type for socket",
  68:"Math result not representable",
  69:"Read only file system",
  70:"Illegal seek",
  71:"No such process",
  72:"Stale file handle",
  73:"Connection timed out",
  74:"Text file busy",
  75:"Cross-device link",
  100:"Device not a stream",
  101:"Bad font file fmt",
  102:"Invalid slot",
  103:"Invalid request code",
  104:"No anode",
  105:"Block device required",
  106:"Channel number out of range",
  107:"Level 3 halted",
  108:"Level 3 reset",
  109:"Link number out of range",
  110:"Protocol driver not attached",
  111:"No CSI structure available",
  112:"Level 2 halted",
  113:"Invalid exchange",
  114:"Invalid request descriptor",
  115:"Exchange full",
  116:"No data (for no delay io)",
  117:"Timer expired",
  118:"Out of streams resources",
  119:"Machine is not on the network",
  120:"Package not installed",
  121:"The object is remote",
  122:"Advertise error",
  123:"Srmount error",
  124:"Communication error on send",
  125:"Cross mount point (not really error)",
  126:"Given log. name not unique",
  127:"f.d. invalid for this operation",
  128:"Remote address changed",
  129:"Can   access a needed shared lib",
  130:"Accessing a corrupted shared lib",
  131:".lib section in a.out corrupted",
  132:"Attempting to link in too many libs",
  133:"Attempting to exec a shared library",
  135:"Streams pipe error",
  136:"Too many users",
  137:"Socket type not supported",
  138:"Not supported",
  139:"Protocol family not supported",
  140:"Can't send after socket shutdown",
  141:"Too many references",
  142:"Host is down",
  148:"No medium (in tape drive)",
  156:"Level 2 not synchronized",
  };
  
  var ERRNO_CODES = {
  };
  
  function demangle(func) {
      warnOnce('warning: build with -sDEMANGLE_SUPPORT to link in libcxxabi demangling');
      return func;
    }
  function demangleAll(text) {
      var regex =
        /\b_Z[\w\d_]+/g;
      return text.replace(regex,
        function(x) {
          var y = demangle(x);
          return x === y ? x : (y + ' [' + x + ']');
        });
    }
  var FS = {
  root:null,
  mounts:[],
  devices:{
  },
  streams:[],
  nextInode:1,
  nameTable:null,
  currentPath:"/",
  initialized:false,
  ignorePermissions:true,
  ErrnoError:null,
  genericErrors:{
  },
  filesystems:null,
  syncFSRequests:0,
  lookupPath:(path, opts = {}) => {
        path = PATH_FS.resolve(path);
  
        if (!path) return { path: '', node: null };
  
        var defaults = {
          follow_mount: true,
          recurse_count: 0
        };
        opts = Object.assign(defaults, opts)
  
        if (opts.recurse_count > 8) {  // max recursive lookup of 8
          throw new FS.ErrnoError(32);
        }
  
        // split the absolute path
        var parts = path.split('/').filter((p) => !!p);
  
        // start at the root
        var current = FS.root;
        var current_path = '/';
  
        for (var i = 0; i < parts.length; i++) {
          var islast = (i === parts.length-1);
          if (islast && opts.parent) {
            // stop resolving
            break;
          }
  
          current = FS.lookupNode(current, parts[i]);
          current_path = PATH.join2(current_path, parts[i]);
  
          // jump to the mount's root node if this is a mountpoint
          if (FS.isMountpoint(current)) {
            if (!islast || (islast && opts.follow_mount)) {
              current = current.mounted.root;
            }
          }
  
          // by default, lookupPath will not follow a symlink if it is the final path component.
          // setting opts.follow = true will override this behavior.
          if (!islast || opts.follow) {
            var count = 0;
            while (FS.isLink(current.mode)) {
              var link = FS.readlink(current_path);
              current_path = PATH_FS.resolve(PATH.dirname(current_path), link);
  
              var lookup = FS.lookupPath(current_path, { recurse_count: opts.recurse_count + 1 });
              current = lookup.node;
  
              if (count++ > 40) {  // limit max consecutive symlinks to 40 (SYMLOOP_MAX).
                throw new FS.ErrnoError(32);
              }
            }
          }
        }
  
        return { path: current_path, node: current };
      },
  getPath:(node) => {
        var path;
        while (true) {
          if (FS.isRoot(node)) {
            var mount = node.mount.mountpoint;
            if (!path) return mount;
            return mount[mount.length-1] !== '/' ? `${mount}/${path}` : mount + path;
          }
          path = path ? `${node.name}/${path}` : node.name;
          node = node.parent;
        }
      },
  hashName:(parentid, name) => {
        var hash = 0;
  
        for (var i = 0; i < name.length; i++) {
          hash = ((hash << 5) - hash + name.charCodeAt(i)) | 0;
        }
        return ((parentid + hash) >>> 0) % FS.nameTable.length;
      },
  hashAddNode:(node) => {
        var hash = FS.hashName(node.parent.id, node.name);
        node.name_next = FS.nameTable[hash];
        FS.nameTable[hash] = node;
      },
  hashRemoveNode:(node) => {
        var hash = FS.hashName(node.parent.id, node.name);
        if (FS.nameTable[hash] === node) {
          FS.nameTable[hash] = node.name_next;
        } else {
          var current = FS.nameTable[hash];
          while (current) {
            if (current.name_next === node) {
              current.name_next = node.name_next;
              break;
            }
            current = current.name_next;
          }
        }
      },
  lookupNode:(parent, name) => {
        var errCode = FS.mayLookup(parent);
        if (errCode) {
          throw new FS.ErrnoError(errCode, parent);
        }
        var hash = FS.hashName(parent.id, name);
        for (var node = FS.nameTable[hash]; node; node = node.name_next) {
          var nodeName = node.name;
          if (node.parent.id === parent.id && nodeName === name) {
            return node;
          }
        }
        // if we failed to find it in the cache, call into the VFS
        return FS.lookup(parent, name);
      },
  createNode:(parent, name, mode, rdev) => {
        assert(typeof parent == 'object')
        var node = new FS.FSNode(parent, name, mode, rdev);
  
        FS.hashAddNode(node);
  
        return node;
      },
  destroyNode:(node) => {
        FS.hashRemoveNode(node);
      },
  isRoot:(node) => {
        return node === node.parent;
      },
  isMountpoint:(node) => {
        return !!node.mounted;
      },
  isFile:(mode) => {
        return (mode & 61440) === 32768;
      },
  isDir:(mode) => {
        return (mode & 61440) === 16384;
      },
  isLink:(mode) => {
        return (mode & 61440) === 40960;
      },
  isChrdev:(mode) => {
        return (mode & 61440) === 8192;
      },
  isBlkdev:(mode) => {
        return (mode & 61440) === 24576;
      },
  isFIFO:(mode) => {
        return (mode & 61440) === 4096;
      },
  isSocket:(mode) => {
        return (mode & 49152) === 49152;
      },
  flagsToPermissionString:(flag) => {
        var perms = ['r', 'w', 'rw'][flag & 3];
        if ((flag & 512)) {
          perms += 'w';
        }
        return perms;
      },
  nodePermissions:(node, perms) => {
        if (FS.ignorePermissions) {
          return 0;
        }
        // return 0 if any user, group or owner bits are set.
        if (perms.includes('r') && !(node.mode & 292)) {
          return 2;
        } else if (perms.includes('w') && !(node.mode & 146)) {
          return 2;
        } else if (perms.includes('x') && !(node.mode & 73)) {
          return 2;
        }
        return 0;
      },
  mayLookup:(dir) => {
        var errCode = FS.nodePermissions(dir, 'x');
        if (errCode) return errCode;
        if (!dir.node_ops.lookup) return 2;
        return 0;
      },
  mayCreate:(dir, name) => {
        try {
          var node = FS.lookupNode(dir, name);
          return 20;
        } catch (e) {
        }
        return FS.nodePermissions(dir, 'wx');
      },
  mayDelete:(dir, name, isdir) => {
        var node;
        try {
          node = FS.lookupNode(dir, name);
        } catch (e) {
          return e.errno;
        }
        var errCode = FS.nodePermissions(dir, 'wx');
        if (errCode) {
          return errCode;
        }
        if (isdir) {
          if (!FS.isDir(node.mode)) {
            return 54;
          }
          if (FS.isRoot(node) || FS.getPath(node) === FS.cwd()) {
            return 10;
          }
        } else {
          if (FS.isDir(node.mode)) {
            return 31;
          }
        }
        return 0;
      },
  mayOpen:(node, flags) => {
        if (!node) {
          return 44;
        }
        if (FS.isLink(node.mode)) {
          return 32;
        } else if (FS.isDir(node.mode)) {
          if (FS.flagsToPermissionString(flags) !== 'r' || // opening for write
              (flags & 512)) { // TODO: check for O_SEARCH? (== search for dir only)
            return 31;
          }
        }
        return FS.nodePermissions(node, FS.flagsToPermissionString(flags));
      },
  MAX_OPEN_FDS:4096,
  nextfd:() => {
        for (var fd = 0; fd <= FS.MAX_OPEN_FDS; fd++) {
          if (!FS.streams[fd]) {
            return fd;
          }
        }
        throw new FS.ErrnoError(33);
      },
  getStreamChecked:(fd) => {
        var stream = FS.getStream(fd);
        if (!stream) {
          throw new FS.ErrnoError(8);
        }
        return stream;
      },
  getStream:(fd) => FS.streams[fd],
  createStream:(stream, fd = -1) => {
        if (!FS.FSStream) {
          FS.FSStream = /** @constructor */ function() {
            this.shared = { };
          };
          FS.FSStream.prototype = {};
          Object.defineProperties(FS.FSStream.prototype, {
            object: {
              /** @this {FS.FSStream} */
              get() { return this.node; },
              /** @this {FS.FSStream} */
              set(val) { this.node = val; }
            },
            isRead: {
              /** @this {FS.FSStream} */
              get() { return (this.flags & 2097155) !== 1; }
            },
            isWrite: {
              /** @this {FS.FSStream} */
              get() { return (this.flags & 2097155) !== 0; }
            },
            isAppend: {
              /** @this {FS.FSStream} */
              get() { return (this.flags & 1024); }
            },
            flags: {
              /** @this {FS.FSStream} */
              get() { return this.shared.flags; },
              /** @this {FS.FSStream} */
              set(val) { this.shared.flags = val; },
            },
            position : {
              /** @this {FS.FSStream} */
              get() { return this.shared.position; },
              /** @this {FS.FSStream} */
              set(val) { this.shared.position = val; },
            },
          });
        }
        // clone it, so we can return an instance of FSStream
        stream = Object.assign(new FS.FSStream(), stream);
        if (fd == -1) {
          fd = FS.nextfd();
        }
        stream.fd = fd;
        FS.streams[fd] = stream;
        return stream;
      },
  closeStream:(fd) => {
        FS.streams[fd] = null;
      },
  chrdev_stream_ops:{
  open:(stream) => {
          var device = FS.getDevice(stream.node.rdev);
          // override node's stream ops with the device's
          stream.stream_ops = device.stream_ops;
          // forward the open call
          if (stream.stream_ops.open) {
            stream.stream_ops.open(stream);
          }
        },
  llseek:() => {
          throw new FS.ErrnoError(70);
        },
  },
  major:(dev) => ((dev) >> 8),
  minor:(dev) => ((dev) & 0xff),
  makedev:(ma, mi) => ((ma) << 8 | (mi)),
  registerDevice:(dev, ops) => {
        FS.devices[dev] = { stream_ops: ops };
      },
  getDevice:(dev) => FS.devices[dev],
  getMounts:(mount) => {
        var mounts = [];
        var check = [mount];
  
        while (check.length) {
          var m = check.pop();
  
          mounts.push(m);
  
          check.push.apply(check, m.mounts);
        }
  
        return mounts;
      },
  syncfs:(populate, callback) => {
        if (typeof populate == 'function') {
          callback = populate;
          populate = false;
        }
  
        FS.syncFSRequests++;
  
        if (FS.syncFSRequests > 1) {
          err(`warning: ${FS.syncFSRequests} FS.syncfs operations in flight at once, probably just doing extra work`);
        }
  
        var mounts = FS.getMounts(FS.root.mount);
        var completed = 0;
  
        function doCallback(errCode) {
          assert(FS.syncFSRequests > 0);
          FS.syncFSRequests--;
          return callback(errCode);
        }
  
        function done(errCode) {
          if (errCode) {
            if (!done.errored) {
              done.errored = true;
              return doCallback(errCode);
            }
            return;
          }
          if (++completed >= mounts.length) {
            doCallback(null);
          }
        };
  
        // sync all mounts
        mounts.forEach((mount) => {
          if (!mount.type.syncfs) {
            return done(null);
          }
          mount.type.syncfs(mount, populate, done);
        });
      },
  mount:(type, opts, mountpoint) => {
        if (typeof type == 'string') {
          // The filesystem was not included, and instead we have an error
          // message stored in the variable.
          throw type;
        }
        var root = mountpoint === '/';
        var pseudo = !mountpoint;
        var node;
  
        if (root && FS.root) {
          throw new FS.ErrnoError(10);
        } else if (!root && !pseudo) {
          var lookup = FS.lookupPath(mountpoint, { follow_mount: false });
  
          mountpoint = lookup.path;  // use the absolute path
          node = lookup.node;
  
          if (FS.isMountpoint(node)) {
            throw new FS.ErrnoError(10);
          }
  
          if (!FS.isDir(node.mode)) {
            throw new FS.ErrnoError(54);
          }
        }
  
        var mount = {
          type,
          opts,
          mountpoint,
          mounts: []
        };
  
        // create a root node for the fs
        var mountRoot = type.mount(mount);
        mountRoot.mount = mount;
        mount.root = mountRoot;
  
        if (root) {
          FS.root = mountRoot;
        } else if (node) {
          // set as a mountpoint
          node.mounted = mount;
  
          // add the new mount to the current mount's children
          if (node.mount) {
            node.mount.mounts.push(mount);
          }
        }
  
        return mountRoot;
      },
  unmount:(mountpoint) => {
        var lookup = FS.lookupPath(mountpoint, { follow_mount: false });
  
        if (!FS.isMountpoint(lookup.node)) {
          throw new FS.ErrnoError(28);
        }
  
        // destroy the nodes for this mount, and all its child mounts
        var node = lookup.node;
        var mount = node.mounted;
        var mounts = FS.getMounts(mount);
  
        Object.keys(FS.nameTable).forEach((hash) => {
          var current = FS.nameTable[hash];
  
          while (current) {
            var next = current.name_next;
  
            if (mounts.includes(current.mount)) {
              FS.destroyNode(current);
            }
  
            current = next;
          }
        });
  
        // no longer a mountpoint
        node.mounted = null;
  
        // remove this mount from the child mounts
        var idx = node.mount.mounts.indexOf(mount);
        assert(idx !== -1);
        node.mount.mounts.splice(idx, 1);
      },
  lookup:(parent, name) => {
        return parent.node_ops.lookup(parent, name);
      },
  mknod:(path, mode, dev) => {
        var lookup = FS.lookupPath(path, { parent: true });
        var parent = lookup.node;
        var name = PATH.basename(path);
        if (!name || name === '.' || name === '..') {
          throw new FS.ErrnoError(28);
        }
        var errCode = FS.mayCreate(parent, name);
        if (errCode) {
          throw new FS.ErrnoError(errCode);
        }
        if (!parent.node_ops.mknod) {
          throw new FS.ErrnoError(63);
        }
        return parent.node_ops.mknod(parent, name, mode, dev);
      },
  create:(path, mode) => {
        mode = mode !== undefined ? mode : 438 /* 0666 */;
        mode &= 4095;
        mode |= 32768;
        return FS.mknod(path, mode, 0);
      },
  mkdir:(path, mode) => {
        mode = mode !== undefined ? mode : 511 /* 0777 */;
        mode &= 511 | 512;
        mode |= 16384;
        return FS.mknod(path, mode, 0);
      },
  mkdirTree:(path, mode) => {
        var dirs = path.split('/');
        var d = '';
        for (var i = 0; i < dirs.length; ++i) {
          if (!dirs[i]) continue;
          d += '/' + dirs[i];
          try {
            FS.mkdir(d, mode);
          } catch(e) {
            if (e.errno != 20) throw e;
          }
        }
      },
  mkdev:(path, mode, dev) => {
        if (typeof dev == 'undefined') {
          dev = mode;
          mode = 438 /* 0666 */;
        }
        mode |= 8192;
        return FS.mknod(path, mode, dev);
      },
  symlink:(oldpath, newpath) => {
        if (!PATH_FS.resolve(oldpath)) {
          throw new FS.ErrnoError(44);
        }
        var lookup = FS.lookupPath(newpath, { parent: true });
        var parent = lookup.node;
        if (!parent) {
          throw new FS.ErrnoError(44);
        }
        var newname = PATH.basename(newpath);
        var errCode = FS.mayCreate(parent, newname);
        if (errCode) {
          throw new FS.ErrnoError(errCode);
        }
        if (!parent.node_ops.symlink) {
          throw new FS.ErrnoError(63);
        }
        return parent.node_ops.symlink(parent, newname, oldpath);
      },
  rename:(old_path, new_path) => {
        var old_dirname = PATH.dirname(old_path);
        var new_dirname = PATH.dirname(new_path);
        var old_name = PATH.basename(old_path);
        var new_name = PATH.basename(new_path);
        // parents must exist
        var lookup, old_dir, new_dir;
  
        // let the errors from non existant directories percolate up
        lookup = FS.lookupPath(old_path, { parent: true });
        old_dir = lookup.node;
        lookup = FS.lookupPath(new_path, { parent: true });
        new_dir = lookup.node;
  
        if (!old_dir || !new_dir) throw new FS.ErrnoError(44);
        // need to be part of the same mount
        if (old_dir.mount !== new_dir.mount) {
          throw new FS.ErrnoError(75);
        }
        // source must exist
        var old_node = FS.lookupNode(old_dir, old_name);
        // old path should not be an ancestor of the new path
        var relative = PATH_FS.relative(old_path, new_dirname);
        if (relative.charAt(0) !== '.') {
          throw new FS.ErrnoError(28);
        }
        // new path should not be an ancestor of the old path
        relative = PATH_FS.relative(new_path, old_dirname);
        if (relative.charAt(0) !== '.') {
          throw new FS.ErrnoError(55);
        }
        // see if the new path already exists
        var new_node;
        try {
          new_node = FS.lookupNode(new_dir, new_name);
        } catch (e) {
          // not fatal
        }
        // early out if nothing needs to change
        if (old_node === new_node) {
          return;
        }
        // we'll need to delete the old entry
        var isdir = FS.isDir(old_node.mode);
        var errCode = FS.mayDelete(old_dir, old_name, isdir);
        if (errCode) {
          throw new FS.ErrnoError(errCode);
        }
        // need delete permissions if we'll be overwriting.
        // need create permissions if new doesn't already exist.
        errCode = new_node ?
          FS.mayDelete(new_dir, new_name, isdir) :
          FS.mayCreate(new_dir, new_name);
        if (errCode) {
          throw new FS.ErrnoError(errCode);
        }
        if (!old_dir.node_ops.rename) {
          throw new FS.ErrnoError(63);
        }
        if (FS.isMountpoint(old_node) || (new_node && FS.isMountpoint(new_node))) {
          throw new FS.ErrnoError(10);
        }
        // if we are going to change the parent, check write permissions
        if (new_dir !== old_dir) {
          errCode = FS.nodePermissions(old_dir, 'w');
          if (errCode) {
            throw new FS.ErrnoError(errCode);
          }
        }
        // remove the node from the lookup hash
        FS.hashRemoveNode(old_node);
        // do the underlying fs rename
        try {
          old_dir.node_ops.rename(old_node, new_dir, new_name);
        } catch (e) {
          throw e;
        } finally {
          // add the node back to the hash (in case node_ops.rename
          // changed its name)
          FS.hashAddNode(old_node);
        }
      },
  rmdir:(path) => {
        var lookup = FS.lookupPath(path, { parent: true });
        var parent = lookup.node;
        var name = PATH.basename(path);
        var node = FS.lookupNode(parent, name);
        var errCode = FS.mayDelete(parent, name, true);
        if (errCode) {
          throw new FS.ErrnoError(errCode);
        }
        if (!parent.node_ops.rmdir) {
          throw new FS.ErrnoError(63);
        }
        if (FS.isMountpoint(node)) {
          throw new FS.ErrnoError(10);
        }
        parent.node_ops.rmdir(parent, name);
        FS.destroyNode(node);
      },
  readdir:(path) => {
        var lookup = FS.lookupPath(path, { follow: true });
        var node = lookup.node;
        if (!node.node_ops.readdir) {
          throw new FS.ErrnoError(54);
        }
        return node.node_ops.readdir(node);
      },
  unlink:(path) => {
        var lookup = FS.lookupPath(path, { parent: true });
        var parent = lookup.node;
        if (!parent) {
          throw new FS.ErrnoError(44);
        }
        var name = PATH.basename(path);
        var node = FS.lookupNode(parent, name);
        var errCode = FS.mayDelete(parent, name, false);
        if (errCode) {
          // According to POSIX, we should map EISDIR to EPERM, but
          // we instead do what Linux does (and we must, as we use
          // the musl linux libc).
          throw new FS.ErrnoError(errCode);
        }
        if (!parent.node_ops.unlink) {
          throw new FS.ErrnoError(63);
        }
        if (FS.isMountpoint(node)) {
          throw new FS.ErrnoError(10);
        }
        parent.node_ops.unlink(parent, name);
        FS.destroyNode(node);
      },
  readlink:(path) => {
        var lookup = FS.lookupPath(path);
        var link = lookup.node;
        if (!link) {
          throw new FS.ErrnoError(44);
        }
        if (!link.node_ops.readlink) {
          throw new FS.ErrnoError(28);
        }
        return PATH_FS.resolve(FS.getPath(link.parent), link.node_ops.readlink(link));
      },
  stat:(path, dontFollow) => {
        var lookup = FS.lookupPath(path, { follow: !dontFollow });
        var node = lookup.node;
        if (!node) {
          throw new FS.ErrnoError(44);
        }
        if (!node.node_ops.getattr) {
          throw new FS.ErrnoError(63);
        }
        return node.node_ops.getattr(node);
      },
  lstat:(path) => {
        return FS.stat(path, true);
      },
  chmod:(path, mode, dontFollow) => {
        var node;
        if (typeof path == 'string') {
          var lookup = FS.lookupPath(path, { follow: !dontFollow });
          node = lookup.node;
        } else {
          node = path;
        }
        if (!node.node_ops.setattr) {
          throw new FS.ErrnoError(63);
        }
        node.node_ops.setattr(node, {
          mode: (mode & 4095) | (node.mode & ~4095),
          timestamp: Date.now()
        });
      },
  lchmod:(path, mode) => {
        FS.chmod(path, mode, true);
      },
  fchmod:(fd, mode) => {
        var stream = FS.getStreamChecked(fd);
        FS.chmod(stream.node, mode);
      },
  chown:(path, uid, gid, dontFollow) => {
        var node;
        if (typeof path == 'string') {
          var lookup = FS.lookupPath(path, { follow: !dontFollow });
          node = lookup.node;
        } else {
          node = path;
        }
        if (!node.node_ops.setattr) {
          throw new FS.ErrnoError(63);
        }
        node.node_ops.setattr(node, {
          timestamp: Date.now()
          // we ignore the uid / gid for now
        });
      },
  lchown:(path, uid, gid) => {
        FS.chown(path, uid, gid, true);
      },
  fchown:(fd, uid, gid) => {
        var stream = FS.getStreamChecked(fd);
        FS.chown(stream.node, uid, gid);
      },
  truncate:(path, len) => {
        if (len < 0) {
          throw new FS.ErrnoError(28);
        }
        var node;
        if (typeof path == 'string') {
          var lookup = FS.lookupPath(path, { follow: true });
          node = lookup.node;
        } else {
          node = path;
        }
        if (!node.node_ops.setattr) {
          throw new FS.ErrnoError(63);
        }
        if (FS.isDir(node.mode)) {
          throw new FS.ErrnoError(31);
        }
        if (!FS.isFile(node.mode)) {
          throw new FS.ErrnoError(28);
        }
        var errCode = FS.nodePermissions(node, 'w');
        if (errCode) {
          throw new FS.ErrnoError(errCode);
        }
        node.node_ops.setattr(node, {
          size: len,
          timestamp: Date.now()
        });
      },
  ftruncate:(fd, len) => {
        var stream = FS.getStreamChecked(fd);
        if ((stream.flags & 2097155) === 0) {
          throw new FS.ErrnoError(28);
        }
        FS.truncate(stream.node, len);
      },
  utime:(path, atime, mtime) => {
        var lookup = FS.lookupPath(path, { follow: true });
        var node = lookup.node;
        node.node_ops.setattr(node, {
          timestamp: Math.max(atime, mtime)
        });
      },
  open:(path, flags, mode) => {
        if (path === "") {
          throw new FS.ErrnoError(44);
        }
        flags = typeof flags == 'string' ? FS_modeStringToFlags(flags) : flags;
        mode = typeof mode == 'undefined' ? 438 /* 0666 */ : mode;
        if ((flags & 64)) {
          mode = (mode & 4095) | 32768;
        } else {
          mode = 0;
        }
        var node;
        if (typeof path == 'object') {
          node = path;
        } else {
          path = PATH.normalize(path);
          try {
            var lookup = FS.lookupPath(path, {
              follow: !(flags & 131072)
            });
            node = lookup.node;
          } catch (e) {
            // ignore
          }
        }
        // perhaps we need to create the node
        var created = false;
        if ((flags & 64)) {
          if (node) {
            // if O_CREAT and O_EXCL are set, error out if the node already exists
            if ((flags & 128)) {
              throw new FS.ErrnoError(20);
            }
          } else {
            // node doesn't exist, try to create it
            node = FS.mknod(path, mode, 0);
            created = true;
          }
        }
        if (!node) {
          throw new FS.ErrnoError(44);
        }
        // can't truncate a device
        if (FS.isChrdev(node.mode)) {
          flags &= ~512;
        }
        // if asked only for a directory, then this must be one
        if ((flags & 65536) && !FS.isDir(node.mode)) {
          throw new FS.ErrnoError(54);
        }
        // check permissions, if this is not a file we just created now (it is ok to
        // create and write to a file with read-only permissions; it is read-only
        // for later use)
        if (!created) {
          var errCode = FS.mayOpen(node, flags);
          if (errCode) {
            throw new FS.ErrnoError(errCode);
          }
        }
        // do truncation if necessary
        if ((flags & 512) && !created) {
          FS.truncate(node, 0);
        }
        // we've already handled these, don't pass down to the underlying vfs
        flags &= ~(128 | 512 | 131072);
  
        // register the stream with the filesystem
        var stream = FS.createStream({
          node,
          path: FS.getPath(node),  // we want the absolute path to the node
          flags,
          seekable: true,
          position: 0,
          stream_ops: node.stream_ops,
          // used by the file family libc calls (fopen, fwrite, ferror, etc.)
          ungotten: [],
          error: false
        });
        // call the new stream's open function
        if (stream.stream_ops.open) {
          stream.stream_ops.open(stream);
        }
        if (Module['logReadFiles'] && !(flags & 1)) {
          if (!FS.readFiles) FS.readFiles = {};
          if (!(path in FS.readFiles)) {
            FS.readFiles[path] = 1;
          }
        }
        return stream;
      },
  close:(stream) => {
        if (FS.isClosed(stream)) {
          throw new FS.ErrnoError(8);
        }
        if (stream.getdents) stream.getdents = null; // free readdir state
        try {
          if (stream.stream_ops.close) {
            stream.stream_ops.close(stream);
          }
        } catch (e) {
          throw e;
        } finally {
          FS.closeStream(stream.fd);
        }
        stream.fd = null;
      },
  isClosed:(stream) => {
        return stream.fd === null;
      },
  llseek:(stream, offset, whence) => {
        if (FS.isClosed(stream)) {
          throw new FS.ErrnoError(8);
        }
        if (!stream.seekable || !stream.stream_ops.llseek) {
          throw new FS.ErrnoError(70);
        }
        if (whence != 0 && whence != 1 && whence != 2) {
          throw new FS.ErrnoError(28);
        }
        stream.position = stream.stream_ops.llseek(stream, offset, whence);
        stream.ungotten = [];
        return stream.position;
      },
  read:(stream, buffer, offset, length, position) => {
        assert(offset >= 0);
        if (length < 0 || position < 0) {
          throw new FS.ErrnoError(28);
        }
        if (FS.isClosed(stream)) {
          throw new FS.ErrnoError(8);
        }
        if ((stream.flags & 2097155) === 1) {
          throw new FS.ErrnoError(8);
        }
        if (FS.isDir(stream.node.mode)) {
          throw new FS.ErrnoError(31);
        }
        if (!stream.stream_ops.read) {
          throw new FS.ErrnoError(28);
        }
        var seeking = typeof position != 'undefined';
        if (!seeking) {
          position = stream.position;
        } else if (!stream.seekable) {
          throw new FS.ErrnoError(70);
        }
        var bytesRead = stream.stream_ops.read(stream, buffer, offset, length, position);
        if (!seeking) stream.position += bytesRead;
        return bytesRead;
      },
  write:(stream, buffer, offset, length, position, canOwn) => {
        assert(offset >= 0);
        if (length < 0 || position < 0) {
          throw new FS.ErrnoError(28);
        }
        if (FS.isClosed(stream)) {
          throw new FS.ErrnoError(8);
        }
        if ((stream.flags & 2097155) === 0) {
          throw new FS.ErrnoError(8);
        }
        if (FS.isDir(stream.node.mode)) {
          throw new FS.ErrnoError(31);
        }
        if (!stream.stream_ops.write) {
          throw new FS.ErrnoError(28);
        }
        if (stream.seekable && stream.flags & 1024) {
          // seek to the end before writing in append mode
          FS.llseek(stream, 0, 2);
        }
        var seeking = typeof position != 'undefined';
        if (!seeking) {
          position = stream.position;
        } else if (!stream.seekable) {
          throw new FS.ErrnoError(70);
        }
        var bytesWritten = stream.stream_ops.write(stream, buffer, offset, length, position, canOwn);
        if (!seeking) stream.position += bytesWritten;
        return bytesWritten;
      },
  allocate:(stream, offset, length) => {
        if (FS.isClosed(stream)) {
          throw new FS.ErrnoError(8);
        }
        if (offset < 0 || length <= 0) {
          throw new FS.ErrnoError(28);
        }
        if ((stream.flags & 2097155) === 0) {
          throw new FS.ErrnoError(8);
        }
        if (!FS.isFile(stream.node.mode) && !FS.isDir(stream.node.mode)) {
          throw new FS.ErrnoError(43);
        }
        if (!stream.stream_ops.allocate) {
          throw new FS.ErrnoError(138);
        }
        stream.stream_ops.allocate(stream, offset, length);
      },
  mmap:(stream, length, position, prot, flags) => {
        // User requests writing to file (prot & PROT_WRITE != 0).
        // Checking if we have permissions to write to the file unless
        // MAP_PRIVATE flag is set. According to POSIX spec it is possible
        // to write to file opened in read-only mode with MAP_PRIVATE flag,
        // as all modifications will be visible only in the memory of
        // the current process.
        if ((prot & 2) !== 0
            && (flags & 2) === 0
            && (stream.flags & 2097155) !== 2) {
          throw new FS.ErrnoError(2);
        }
        if ((stream.flags & 2097155) === 1) {
          throw new FS.ErrnoError(2);
        }
        if (!stream.stream_ops.mmap) {
          throw new FS.ErrnoError(43);
        }
        return stream.stream_ops.mmap(stream, length, position, prot, flags);
      },
  msync:(stream, buffer, offset, length, mmapFlags) => {
        assert(offset >= 0);
        if (!stream.stream_ops.msync) {
          return 0;
        }
        return stream.stream_ops.msync(stream, buffer, offset, length, mmapFlags);
      },
  munmap:(stream) => 0,
  ioctl:(stream, cmd, arg) => {
        if (!stream.stream_ops.ioctl) {
          throw new FS.ErrnoError(59);
        }
        return stream.stream_ops.ioctl(stream, cmd, arg);
      },
  readFile:(path, opts = {}) => {
        opts.flags = opts.flags || 0;
        opts.encoding = opts.encoding || 'binary';
        if (opts.encoding !== 'utf8' && opts.encoding !== 'binary') {
          throw new Error(`Invalid encoding type "${opts.encoding}"`);
        }
        var ret;
        var stream = FS.open(path, opts.flags);
        var stat = FS.stat(path);
        var length = stat.size;
        var buf = new Uint8Array(length);
        FS.read(stream, buf, 0, length, 0);
        if (opts.encoding === 'utf8') {
          ret = UTF8ArrayToString(buf, 0);
        } else if (opts.encoding === 'binary') {
          ret = buf;
        }
        FS.close(stream);
        return ret;
      },
  writeFile:(path, data, opts = {}) => {
        opts.flags = opts.flags || 577;
        var stream = FS.open(path, opts.flags, opts.mode);
        if (typeof data == 'string') {
          var buf = new Uint8Array(lengthBytesUTF8(data)+1);
          var actualNumBytes = stringToUTF8Array(data, buf, 0, buf.length);
          FS.write(stream, buf, 0, actualNumBytes, undefined, opts.canOwn);
        } else if (ArrayBuffer.isView(data)) {
          FS.write(stream, data, 0, data.byteLength, undefined, opts.canOwn);
        } else {
          throw new Error('Unsupported data type');
        }
        FS.close(stream);
      },
  cwd:() => FS.currentPath,
  chdir:(path) => {
        var lookup = FS.lookupPath(path, { follow: true });
        if (lookup.node === null) {
          throw new FS.ErrnoError(44);
        }
        if (!FS.isDir(lookup.node.mode)) {
          throw new FS.ErrnoError(54);
        }
        var errCode = FS.nodePermissions(lookup.node, 'x');
        if (errCode) {
          throw new FS.ErrnoError(errCode);
        }
        FS.currentPath = lookup.path;
      },
  createDefaultDirectories:() => {
        FS.mkdir('/tmp');
        FS.mkdir('/home');
        FS.mkdir('/home/web_user');
      },
  createDefaultDevices:() => {
        // create /dev
        FS.mkdir('/dev');
        // setup /dev/null
        FS.registerDevice(FS.makedev(1, 3), {
          read: () => 0,
          write: (stream, buffer, offset, length, pos) => length,
        });
        FS.mkdev('/dev/null', FS.makedev(1, 3));
        // setup /dev/tty and /dev/tty1
        // stderr needs to print output using err() rather than out()
        // so we register a second tty just for it.
        TTY.register(FS.makedev(5, 0), TTY.default_tty_ops);
        TTY.register(FS.makedev(6, 0), TTY.default_tty1_ops);
        FS.mkdev('/dev/tty', FS.makedev(5, 0));
        FS.mkdev('/dev/tty1', FS.makedev(6, 0));
        // setup /dev/[u]random
        // use a buffer to avoid overhead of individual crypto calls per byte
        var randomBuffer = new Uint8Array(1024), randomLeft = 0;
        var randomByte = () => {
          if (randomLeft === 0) {
            randomLeft = randomFill(randomBuffer).byteLength;
          }
          return randomBuffer[--randomLeft];
        };
        FS.createDevice('/dev', 'random', randomByte);
        FS.createDevice('/dev', 'urandom', randomByte);
        // we're not going to emulate the actual shm device,
        // just create the tmp dirs that reside in it commonly
        FS.mkdir('/dev/shm');
        FS.mkdir('/dev/shm/tmp');
      },
  createSpecialDirectories:() => {
        // create /proc/self/fd which allows /proc/self/fd/6 => readlink gives the
        // name of the stream for fd 6 (see test_unistd_ttyname)
        FS.mkdir('/proc');
        var proc_self = FS.mkdir('/proc/self');
        FS.mkdir('/proc/self/fd');
        FS.mount({
          mount: () => {
            var node = FS.createNode(proc_self, 'fd', 16384 | 511 /* 0777 */, 73);
            node.node_ops = {
              lookup: (parent, name) => {
                var fd = +name;
                var stream = FS.getStreamChecked(fd);
                var ret = {
                  parent: null,
                  mount: { mountpoint: 'fake' },
                  node_ops: { readlink: () => stream.path },
                };
                ret.parent = ret; // make it look like a simple root node
                return ret;
              }
            };
            return node;
          }
        }, {}, '/proc/self/fd');
      },
  createStandardStreams:() => {
        // TODO deprecate the old functionality of a single
        // input / output callback and that utilizes FS.createDevice
        // and instead require a unique set of stream ops
  
        // by default, we symlink the standard streams to the
        // default tty devices. however, if the standard streams
        // have been overwritten we create a unique device for
        // them instead.
        if (Module['stdin']) {
          FS.createDevice('/dev', 'stdin', Module['stdin']);
        } else {
          FS.symlink('/dev/tty', '/dev/stdin');
        }
        if (Module['stdout']) {
          FS.createDevice('/dev', 'stdout', null, Module['stdout']);
        } else {
          FS.symlink('/dev/tty', '/dev/stdout');
        }
        if (Module['stderr']) {
          FS.createDevice('/dev', 'stderr', null, Module['stderr']);
        } else {
          FS.symlink('/dev/tty1', '/dev/stderr');
        }
  
        // open default streams for the stdin, stdout and stderr devices
        var stdin = FS.open('/dev/stdin', 0);
        var stdout = FS.open('/dev/stdout', 1);
        var stderr = FS.open('/dev/stderr', 1);
        assert(stdin.fd === 0, `invalid handle for stdin (${stdin.fd})`);
        assert(stdout.fd === 1, `invalid handle for stdout (${stdout.fd})`);
        assert(stderr.fd === 2, `invalid handle for stderr (${stderr.fd})`);
      },
  ensureErrnoError:() => {
        if (FS.ErrnoError) return;
        FS.ErrnoError = /** @this{Object} */ function ErrnoError(errno, node) {
          // We set the `name` property to be able to identify `FS.ErrnoError`
          // - the `name` is a standard ECMA-262 property of error objects. Kind of good to have it anyway.
          // - when using PROXYFS, an error can come from an underlying FS
          // as different FS objects have their own FS.ErrnoError each,
          // the test `err instanceof FS.ErrnoError` won't detect an error coming from another filesystem, causing bugs.
          // we'll use the reliable test `err.name == "ErrnoError"` instead
          this.name = 'ErrnoError';
          this.node = node;
          this.setErrno = /** @this{Object} */ function(errno) {
            this.errno = errno;
            for (var key in ERRNO_CODES) {
              if (ERRNO_CODES[key] === errno) {
                this.code = key;
                break;
              }
            }
          };
          this.setErrno(errno);
          this.message = ERRNO_MESSAGES[errno];
  
          // Try to get a maximally helpful stack trace. On Node.js, getting Error.stack
          // now ensures it shows what we want.
          if (this.stack) {
            // Define the stack property for Node.js 4, which otherwise errors on the next line.
            Object.defineProperty(this, "stack", { value: (new Error).stack, writable: true });
            this.stack = demangleAll(this.stack);
          }
        };
        FS.ErrnoError.prototype = new Error();
        FS.ErrnoError.prototype.constructor = FS.ErrnoError;
        // Some errors may happen quite a bit, to avoid overhead we reuse them (and suffer a lack of stack info)
        [44].forEach((code) => {
          FS.genericErrors[code] = new FS.ErrnoError(code);
          FS.genericErrors[code].stack = '<generic error, no stack>';
        });
      },
  staticInit:() => {
        FS.ensureErrnoError();
  
        FS.nameTable = new Array(4096);
  
        FS.mount(MEMFS, {}, '/');
  
        FS.createDefaultDirectories();
        FS.createDefaultDevices();
        FS.createSpecialDirectories();
  
        FS.filesystems = {
          'MEMFS': MEMFS,
        };
      },
  init:(input, output, error) => {
        assert(!FS.init.initialized, 'FS.init was previously called. If you want to initialize later with custom parameters, remove any earlier calls (note that one is automatically added to the generated code)');
        FS.init.initialized = true;
  
        FS.ensureErrnoError();
  
        // Allow Module.stdin etc. to provide defaults, if none explicitly passed to us here
        Module['stdin'] = input || Module['stdin'];
        Module['stdout'] = output || Module['stdout'];
        Module['stderr'] = error || Module['stderr'];
  
        FS.createStandardStreams();
      },
  quit:() => {
        FS.init.initialized = false;
        // force-flush all streams, so we get musl std streams printed out
        _fflush(0);
        // close all of our streams
        for (var i = 0; i < FS.streams.length; i++) {
          var stream = FS.streams[i];
          if (!stream) {
            continue;
          }
          FS.close(stream);
        }
      },
  findObject:(path, dontResolveLastLink) => {
        var ret = FS.analyzePath(path, dontResolveLastLink);
        if (!ret.exists) {
          return null;
        }
        return ret.object;
      },
  analyzePath:(path, dontResolveLastLink) => {
        // operate from within the context of the symlink's target
        try {
          var lookup = FS.lookupPath(path, { follow: !dontResolveLastLink });
          path = lookup.path;
        } catch (e) {
        }
        var ret = {
          isRoot: false, exists: false, error: 0, name: null, path: null, object: null,
          parentExists: false, parentPath: null, parentObject: null
        };
        try {
          var lookup = FS.lookupPath(path, { parent: true });
          ret.parentExists = true;
          ret.parentPath = lookup.path;
          ret.parentObject = lookup.node;
          ret.name = PATH.basename(path);
          lookup = FS.lookupPath(path, { follow: !dontResolveLastLink });
          ret.exists = true;
          ret.path = lookup.path;
          ret.object = lookup.node;
          ret.name = lookup.node.name;
          ret.isRoot = lookup.path === '/';
        } catch (e) {
          ret.error = e.errno;
        };
        return ret;
      },
  createPath:(parent, path, canRead, canWrite) => {
        parent = typeof parent == 'string' ? parent : FS.getPath(parent);
        var parts = path.split('/').reverse();
        while (parts.length) {
          var part = parts.pop();
          if (!part) continue;
          var current = PATH.join2(parent, part);
          try {
            FS.mkdir(current);
          } catch (e) {
            // ignore EEXIST
          }
          parent = current;
        }
        return current;
      },
  createFile:(parent, name, properties, canRead, canWrite) => {
        var path = PATH.join2(typeof parent == 'string' ? parent : FS.getPath(parent), name);
        var mode = FS_getMode(canRead, canWrite);
        return FS.create(path, mode);
      },
  createDataFile:(parent, name, data, canRead, canWrite, canOwn) => {
        var path = name;
        if (parent) {
          parent = typeof parent == 'string' ? parent : FS.getPath(parent);
          path = name ? PATH.join2(parent, name) : parent;
        }
        var mode = FS_getMode(canRead, canWrite);
        var node = FS.create(path, mode);
        if (data) {
          if (typeof data == 'string') {
            var arr = new Array(data.length);
            for (var i = 0, len = data.length; i < len; ++i) arr[i] = data.charCodeAt(i);
            data = arr;
          }
          // make sure we can write to the file
          FS.chmod(node, mode | 146);
          var stream = FS.open(node, 577);
          FS.write(stream, data, 0, data.length, 0, canOwn);
          FS.close(stream);
          FS.chmod(node, mode);
        }
        return node;
      },
  createDevice:(parent, name, input, output) => {
        var path = PATH.join2(typeof parent == 'string' ? parent : FS.getPath(parent), name);
        var mode = FS_getMode(!!input, !!output);
        if (!FS.createDevice.major) FS.createDevice.major = 64;
        var dev = FS.makedev(FS.createDevice.major++, 0);
        // Create a fake device that a set of stream ops to emulate
        // the old behavior.
        FS.registerDevice(dev, {
          open: (stream) => {
            stream.seekable = false;
          },
          close: (stream) => {
            // flush any pending line data
            if (output && output.buffer && output.buffer.length) {
              output(10);
            }
          },
          read: (stream, buffer, offset, length, pos /* ignored */) => {
            var bytesRead = 0;
            for (var i = 0; i < length; i++) {
              var result;
              try {
                result = input();
              } catch (e) {
                throw new FS.ErrnoError(29);
              }
              if (result === undefined && bytesRead === 0) {
                throw new FS.ErrnoError(6);
              }
              if (result === null || result === undefined) break;
              bytesRead++;
              buffer[offset+i] = result;
            }
            if (bytesRead) {
              stream.node.timestamp = Date.now();
            }
            return bytesRead;
          },
          write: (stream, buffer, offset, length, pos) => {
            for (var i = 0; i < length; i++) {
              try {
                output(buffer[offset+i]);
              } catch (e) {
                throw new FS.ErrnoError(29);
              }
            }
            if (length) {
              stream.node.timestamp = Date.now();
            }
            return i;
          }
        });
        return FS.mkdev(path, mode, dev);
      },
  forceLoadFile:(obj) => {
        if (obj.isDevice || obj.isFolder || obj.link || obj.contents) return true;
        if (typeof XMLHttpRequest != 'undefined') {
          throw new Error("Lazy loading should have been performed (contents set) in createLazyFile, but it was not. Lazy loading only works in web workers. Use --embed-file or --preload-file in emcc on the main thread.");
        } else if (read_) {
          // Command-line.
          try {
            // WARNING: Can't read binary files in V8's d8 or tracemonkey's js, as
            //          read() will try to parse UTF8.
            obj.contents = intArrayFromString(read_(obj.url), true);
            obj.usedBytes = obj.contents.length;
          } catch (e) {
            throw new FS.ErrnoError(29);
          }
        } else {
          throw new Error('Cannot load without read() or XMLHttpRequest.');
        }
      },
  createLazyFile:(parent, name, url, canRead, canWrite) => {
        // Lazy chunked Uint8Array (implements get and length from Uint8Array). Actual getting is abstracted away for eventual reuse.
        /** @constructor */
        function LazyUint8Array() {
          this.lengthKnown = false;
          this.chunks = []; // Loaded chunks. Index is the chunk number
        }
        LazyUint8Array.prototype.get = /** @this{Object} */ function LazyUint8Array_get(idx) {
          if (idx > this.length-1 || idx < 0) {
            return undefined;
          }
          var chunkOffset = idx % this.chunkSize;
          var chunkNum = (idx / this.chunkSize)|0;
          return this.getter(chunkNum)[chunkOffset];
        };
        LazyUint8Array.prototype.setDataGetter = function LazyUint8Array_setDataGetter(getter) {
          this.getter = getter;
        };
        LazyUint8Array.prototype.cacheLength = function LazyUint8Array_cacheLength() {
          // Find length
          var xhr = new XMLHttpRequest();
          xhr.open('HEAD', url, false);
          xhr.send(null);
          if (!(xhr.status >= 200 && xhr.status < 300 || xhr.status === 304)) throw new Error("Couldn't load " + url + ". Status: " + xhr.status);
          var datalength = Number(xhr.getResponseHeader("Content-length"));
          var header;
          var hasByteServing = (header = xhr.getResponseHeader("Accept-Ranges")) && header === "bytes";
          var usesGzip = (header = xhr.getResponseHeader("Content-Encoding")) && header === "gzip";
  
          var chunkSize = 1024*1024; // Chunk size in bytes
  
          if (!hasByteServing) chunkSize = datalength;
  
          // Function to get a range from the remote URL.
          var doXHR = (from, to) => {
            if (from > to) throw new Error("invalid range (" + from + ", " + to + ") or no bytes requested!");
            if (to > datalength-1) throw new Error("only " + datalength + " bytes available! programmer error!");
  
            // TODO: Use mozResponseArrayBuffer, responseStream, etc. if available.
            var xhr = new XMLHttpRequest();
            xhr.open('GET', url, false);
            if (datalength !== chunkSize) xhr.setRequestHeader("Range", "bytes=" + from + "-" + to);
  
            // Some hints to the browser that we want binary data.
            xhr.responseType = 'arraybuffer';
            if (xhr.overrideMimeType) {
              xhr.overrideMimeType('text/plain; charset=x-user-defined');
            }
  
            xhr.send(null);
            if (!(xhr.status >= 200 && xhr.status < 300 || xhr.status === 304)) throw new Error("Couldn't load " + url + ". Status: " + xhr.status);
            if (xhr.response !== undefined) {
              return new Uint8Array(/** @type{Array<number>} */(xhr.response || []));
            }
            return intArrayFromString(xhr.responseText || '', true);
          };
          var lazyArray = this;
          lazyArray.setDataGetter((chunkNum) => {
            var start = chunkNum * chunkSize;
            var end = (chunkNum+1) * chunkSize - 1; // including this byte
            end = Math.min(end, datalength-1); // if datalength-1 is selected, this is the last block
            if (typeof lazyArray.chunks[chunkNum] == 'undefined') {
              lazyArray.chunks[chunkNum] = doXHR(start, end);
            }
            if (typeof lazyArray.chunks[chunkNum] == 'undefined') throw new Error('doXHR failed!');
            return lazyArray.chunks[chunkNum];
          });
  
          if (usesGzip || !datalength) {
            // if the server uses gzip or doesn't supply the length, we have to download the whole file to get the (uncompressed) length
            chunkSize = datalength = 1; // this will force getter(0)/doXHR do download the whole file
            datalength = this.getter(0).length;
            chunkSize = datalength;
            out("LazyFiles on gzip forces download of the whole file when length is accessed");
          }
  
          this._length = datalength;
          this._chunkSize = chunkSize;
          this.lengthKnown = true;
        };
        if (typeof XMLHttpRequest != 'undefined') {
          if (!ENVIRONMENT_IS_WORKER) throw 'Cannot do synchronous binary XHRs outside webworkers in modern browsers. Use --embed-file or --preload-file in emcc';
          var lazyArray = new LazyUint8Array();
          Object.defineProperties(lazyArray, {
            length: {
              get: /** @this{Object} */ function() {
                if (!this.lengthKnown) {
                  this.cacheLength();
                }
                return this._length;
              }
            },
            chunkSize: {
              get: /** @this{Object} */ function() {
                if (!this.lengthKnown) {
                  this.cacheLength();
                }
                return this._chunkSize;
              }
            }
          });
  
          var properties = { isDevice: false, contents: lazyArray };
        } else {
          var properties = { isDevice: false, url: url };
        }
  
        var node = FS.createFile(parent, name, properties, canRead, canWrite);
        // This is a total hack, but I want to get this lazy file code out of the
        // core of MEMFS. If we want to keep this lazy file concept I feel it should
        // be its own thin LAZYFS proxying calls to MEMFS.
        if (properties.contents) {
          node.contents = properties.contents;
        } else if (properties.url) {
          node.contents = null;
          node.url = properties.url;
        }
        // Add a function that defers querying the file size until it is asked the first time.
        Object.defineProperties(node, {
          usedBytes: {
            get: /** @this {FSNode} */ function() { return this.contents.length; }
          }
        });
        // override each stream op with one that tries to force load the lazy file first
        var stream_ops = {};
        var keys = Object.keys(node.stream_ops);
        keys.forEach((key) => {
          var fn = node.stream_ops[key];
          stream_ops[key] = function forceLoadLazyFile() {
            FS.forceLoadFile(node);
            return fn.apply(null, arguments);
          };
        });
        function writeChunks(stream, buffer, offset, length, position) {
          var contents = stream.node.contents;
          if (position >= contents.length)
            return 0;
          var size = Math.min(contents.length - position, length);
          assert(size >= 0);
          if (contents.slice) { // normal array
            for (var i = 0; i < size; i++) {
              buffer[offset + i] = contents[position + i];
            }
          } else {
            for (var i = 0; i < size; i++) { // LazyUint8Array from sync binary XHR
              buffer[offset + i] = contents.get(position + i);
            }
          }
          return size;
        }
        // use a custom read function
        stream_ops.read = (stream, buffer, offset, length, position) => {
          FS.forceLoadFile(node);
          return writeChunks(stream, buffer, offset, length, position)
        };
        // use a custom mmap function
        stream_ops.mmap = (stream, length, position, prot, flags) => {
          FS.forceLoadFile(node);
          var ptr = mmapAlloc(length);
          if (!ptr) {
            throw new FS.ErrnoError(48);
          }
          writeChunks(stream, HEAP8, ptr, length, position);
          return { ptr, allocated: true };
        };
        node.stream_ops = stream_ops;
        return node;
      },
  absolutePath:() => {
        abort('FS.absolutePath has been removed; use PATH_FS.resolve instead');
      },
  createFolder:() => {
        abort('FS.createFolder has been removed; use FS.mkdir instead');
      },
  createLink:() => {
        abort('FS.createLink has been removed; use FS.symlink instead');
      },
  joinPath:() => {
        abort('FS.joinPath has been removed; use PATH.join instead');
      },
  mmapAlloc:() => {
        abort('FS.mmapAlloc has been replaced by the top level function mmapAlloc');
      },
  standardizePath:() => {
        abort('FS.standardizePath has been removed; use PATH.normalize instead');
      },
  };
  
  
    /**
     * Given a pointer 'ptr' to a null-terminated UTF8-encoded string in the
     * emscripten HEAP, returns a copy of that string as a Javascript String object.
     *
     * @param {number} ptr
     * @param {number=} maxBytesToRead - An optional length that specifies the
     *   maximum number of bytes to read. You can omit this parameter to scan the
     *   string until the first 0 byte. If maxBytesToRead is passed, and the string
     *   at [ptr, ptr+maxBytesToReadr[ contains a null byte in the middle, then the
     *   string will cut short at that byte index (i.e. maxBytesToRead will not
     *   produce a string of exact length [ptr, ptr+maxBytesToRead[) N.B. mixing
     *   frequent uses of UTF8ToString() with and without maxBytesToRead may throw
     *   JS JIT optimizations off, so it is worth to consider consistently using one
     * @return {string}
     */
  var UTF8ToString = (ptr, maxBytesToRead) => {
      assert(typeof ptr == 'number');
      return ptr ? UTF8ArrayToString(HEAPU8, ptr, maxBytesToRead) : '';
    };
  var SYSCALLS = {
  DEFAULT_POLLMASK:5,
  calculateAt:function(dirfd, path, allowEmpty) {
        if (PATH.isAbs(path)) {
          return path;
        }
        // relative path
        var dir;
        if (dirfd === -100) {
          dir = FS.cwd();
        } else {
          var dirstream = SYSCALLS.getStreamFromFD(dirfd);
          dir = dirstream.path;
        }
        if (path.length == 0) {
          if (!allowEmpty) {
            throw new FS.ErrnoError(44);;
          }
          return dir;
        }
        return PATH.join2(dir, path);
      },
  doStat:function(func, path, buf) {
        try {
          var stat = func(path);
        } catch (e) {
          if (e && e.node && PATH.normalize(path) !== PATH.normalize(FS.getPath(e.node))) {
            // an error occurred while trying to look up the path; we should just report ENOTDIR
            return -54;
          }
          throw e;
        }
        HEAP32[((buf)>>2)] = stat.dev;
        HEAP32[(((buf)+(4))>>2)] = stat.mode;
        HEAPU32[(((buf)+(8))>>2)] = stat.nlink;
        HEAP32[(((buf)+(12))>>2)] = stat.uid;
        HEAP32[(((buf)+(16))>>2)] = stat.gid;
        HEAP32[(((buf)+(20))>>2)] = stat.rdev;
        (tempI64 = [stat.size>>>0,(tempDouble=stat.size,(+(Math.abs(tempDouble))) >= 1.0 ? (tempDouble > 0.0 ? (+(Math.floor((tempDouble)/4294967296.0)))>>>0 : (~~((+(Math.ceil((tempDouble - +(((~~(tempDouble)))>>>0))/4294967296.0)))))>>>0) : 0)], HEAP32[(((buf)+(24))>>2)] = tempI64[0],HEAP32[(((buf)+(28))>>2)] = tempI64[1]);
        HEAP32[(((buf)+(32))>>2)] = 4096;
        HEAP32[(((buf)+(36))>>2)] = stat.blocks;
        var atime = stat.atime.getTime();
        var mtime = stat.mtime.getTime();
        var ctime = stat.ctime.getTime();
        (tempI64 = [Math.floor(atime / 1000)>>>0,(tempDouble=Math.floor(atime / 1000),(+(Math.abs(tempDouble))) >= 1.0 ? (tempDouble > 0.0 ? (+(Math.floor((tempDouble)/4294967296.0)))>>>0 : (~~((+(Math.ceil((tempDouble - +(((~~(tempDouble)))>>>0))/4294967296.0)))))>>>0) : 0)], HEAP32[(((buf)+(40))>>2)] = tempI64[0],HEAP32[(((buf)+(44))>>2)] = tempI64[1]);
        HEAPU32[(((buf)+(48))>>2)] = (atime % 1000) * 1000;
        (tempI64 = [Math.floor(mtime / 1000)>>>0,(tempDouble=Math.floor(mtime / 1000),(+(Math.abs(tempDouble))) >= 1.0 ? (tempDouble > 0.0 ? (+(Math.floor((tempDouble)/4294967296.0)))>>>0 : (~~((+(Math.ceil((tempDouble - +(((~~(tempDouble)))>>>0))/4294967296.0)))))>>>0) : 0)], HEAP32[(((buf)+(56))>>2)] = tempI64[0],HEAP32[(((buf)+(60))>>2)] = tempI64[1]);
        HEAPU32[(((buf)+(64))>>2)] = (mtime % 1000) * 1000;
        (tempI64 = [Math.floor(ctime / 1000)>>>0,(tempDouble=Math.floor(ctime / 1000),(+(Math.abs(tempDouble))) >= 1.0 ? (tempDouble > 0.0 ? (+(Math.floor((tempDouble)/4294967296.0)))>>>0 : (~~((+(Math.ceil((tempDouble - +(((~~(tempDouble)))>>>0))/4294967296.0)))))>>>0) : 0)], HEAP32[(((buf)+(72))>>2)] = tempI64[0],HEAP32[(((buf)+(76))>>2)] = tempI64[1]);
        HEAPU32[(((buf)+(80))>>2)] = (ctime % 1000) * 1000;
        (tempI64 = [stat.ino>>>0,(tempDouble=stat.ino,(+(Math.abs(tempDouble))) >= 1.0 ? (tempDouble > 0.0 ? (+(Math.floor((tempDouble)/4294967296.0)))>>>0 : (~~((+(Math.ceil((tempDouble - +(((~~(tempDouble)))>>>0))/4294967296.0)))))>>>0) : 0)], HEAP32[(((buf)+(88))>>2)] = tempI64[0],HEAP32[(((buf)+(92))>>2)] = tempI64[1]);
        return 0;
      },
  doMsync:function(addr, stream, len, flags, offset) {
        if (!FS.isFile(stream.node.mode)) {
          throw new FS.ErrnoError(43);
        }
        if (flags & 2) {
          // MAP_PRIVATE calls need not to be synced back to underlying fs
          return 0;
        }
        var buffer = HEAPU8.slice(addr, addr + len);
        FS.msync(stream, buffer, offset, len, flags);
      },
  varargs:undefined,
  get() {
        assert(SYSCALLS.varargs != undefined);
        SYSCALLS.varargs += 4;
        var ret = HEAP32[(((SYSCALLS.varargs)-(4))>>2)];
        return ret;
      },
  getStr(ptr) {
        var ret = UTF8ToString(ptr);
        return ret;
      },
  getStreamFromFD:function(fd) {
        var stream = FS.getStreamChecked(fd);
        return stream;
      },
  };
  function ___syscall_fcntl64(fd, cmd, varargs) {
  SYSCALLS.varargs = varargs;
  try {
  
      var stream = SYSCALLS.getStreamFromFD(fd);
      switch (cmd) {
        case 0: {
          var arg = SYSCALLS.get();
          if (arg < 0) {
            return -28;
          }
          var newStream;
          newStream = FS.createStream(stream, arg);
          return newStream.fd;
        }
        case 1:
        case 2:
          return 0;  // FD_CLOEXEC makes no sense for a single process.
        case 3:
          return stream.flags;
        case 4: {
          var arg = SYSCALLS.get();
          stream.flags |= arg;
          return 0;
        }
        case 5: {
          var arg = SYSCALLS.get();
          var offset = 0;
          // We're always unlocked.
          HEAP16[(((arg)+(offset))>>1)] = 2;
          return 0;
        }
        case 6:
        case 7:
          return 0; // Pretend that the locking is successful.
        case 16:
        case 8:
          return -28; // These are for sockets. We don't have them fully implemented yet.
        case 9:
          // musl trusts getown return values, due to a bug where they must be, as they overlap with errors. just return -1 here, so fcntl() returns that, and we set errno ourselves.
          setErrNo(28);
          return -1;
        default: {
          return -28;
        }
      }
    } catch (e) {
    if (typeof FS == 'undefined' || !(e.name === 'ErrnoError')) throw e;
    return -e.errno;
  }
  }

  function ___syscall_fstat64(fd, buf) {
  try {
  
      var stream = SYSCALLS.getStreamFromFD(fd);
      return SYSCALLS.doStat(FS.stat, stream.path, buf);
    } catch (e) {
    if (typeof FS == 'undefined' || !(e.name === 'ErrnoError')) throw e;
    return -e.errno;
  }
  }

  var stringToUTF8 = (str, outPtr, maxBytesToWrite) => {
      assert(typeof maxBytesToWrite == 'number', 'stringToUTF8(str, outPtr, maxBytesToWrite) is missing the third parameter that specifies the length of the output buffer!');
      return stringToUTF8Array(str, HEAPU8,outPtr, maxBytesToWrite);
    };
  
  function ___syscall_getdents64(fd, dirp, count) {
  try {
  
      var stream = SYSCALLS.getStreamFromFD(fd)
      if (!stream.getdents) {
        stream.getdents = FS.readdir(stream.path);
      }
  
      var struct_size = 280;
      var pos = 0;
      var off = FS.llseek(stream, 0, 1);
  
      var idx = Math.floor(off / struct_size);
  
      while (idx < stream.getdents.length && pos + struct_size <= count) {
        var id;
        var type;
        var name = stream.getdents[idx];
        if (name === '.') {
          id = stream.node.id;
          type = 4; // DT_DIR
        }
        else if (name === '..') {
          var lookup = FS.lookupPath(stream.path, { parent: true });
          id = lookup.node.id;
          type = 4; // DT_DIR
        }
        else {
          var child = FS.lookupNode(stream.node, name);
          id = child.id;
          type = FS.isChrdev(child.mode) ? 2 :  // DT_CHR, character device.
                 FS.isDir(child.mode) ? 4 :     // DT_DIR, directory.
                 FS.isLink(child.mode) ? 10 :   // DT_LNK, symbolic link.
                 8;                             // DT_REG, regular file.
        }
        assert(id);
        (tempI64 = [id>>>0,(tempDouble=id,(+(Math.abs(tempDouble))) >= 1.0 ? (tempDouble > 0.0 ? (+(Math.floor((tempDouble)/4294967296.0)))>>>0 : (~~((+(Math.ceil((tempDouble - +(((~~(tempDouble)))>>>0))/4294967296.0)))))>>>0) : 0)], HEAP32[((dirp + pos)>>2)] = tempI64[0],HEAP32[(((dirp + pos)+(4))>>2)] = tempI64[1]);
        (tempI64 = [(idx + 1) * struct_size>>>0,(tempDouble=(idx + 1) * struct_size,(+(Math.abs(tempDouble))) >= 1.0 ? (tempDouble > 0.0 ? (+(Math.floor((tempDouble)/4294967296.0)))>>>0 : (~~((+(Math.ceil((tempDouble - +(((~~(tempDouble)))>>>0))/4294967296.0)))))>>>0) : 0)], HEAP32[(((dirp + pos)+(8))>>2)] = tempI64[0],HEAP32[(((dirp + pos)+(12))>>2)] = tempI64[1]);
        HEAP16[(((dirp + pos)+(16))>>1)] = 280;
        HEAP8[(((dirp + pos)+(18))>>0)] = type;
        stringToUTF8(name, dirp + pos + 19, 256);
        pos += struct_size;
        idx += 1;
      }
      FS.llseek(stream, idx * struct_size, 0);
      return pos;
    } catch (e) {
    if (typeof FS == 'undefined' || !(e.name === 'ErrnoError')) throw e;
    return -e.errno;
  }
  }

  function ___syscall_ioctl(fd, op, varargs) {
  SYSCALLS.varargs = varargs;
  try {
  
      var stream = SYSCALLS.getStreamFromFD(fd);
      switch (op) {
        case 21509: {
          if (!stream.tty) return -59;
          return 0;
        }
        case 21505: {
          if (!stream.tty) return -59;
          if (stream.tty.ops.ioctl_tcgets) {
            var termios = stream.tty.ops.ioctl_tcgets(stream);
            var argp = SYSCALLS.get();
            HEAP32[((argp)>>2)] = termios.c_iflag || 0;
            HEAP32[(((argp)+(4))>>2)] = termios.c_oflag || 0;
            HEAP32[(((argp)+(8))>>2)] = termios.c_cflag || 0;
            HEAP32[(((argp)+(12))>>2)] = termios.c_lflag || 0;
            for (var i = 0; i < 32; i++) {
              HEAP8[(((argp + i)+(17))>>0)] = termios.c_cc[i] || 0;
            }
            return 0;
          }
          return 0;
        }
        case 21510:
        case 21511:
        case 21512: {
          if (!stream.tty) return -59;
          return 0; // no-op, not actually adjusting terminal settings
        }
        case 21506:
        case 21507:
        case 21508: {
          if (!stream.tty) return -59;
          if (stream.tty.ops.ioctl_tcsets) {
            var argp = SYSCALLS.get();
            var c_iflag = HEAP32[((argp)>>2)];
            var c_oflag = HEAP32[(((argp)+(4))>>2)];
            var c_cflag = HEAP32[(((argp)+(8))>>2)];
            var c_lflag = HEAP32[(((argp)+(12))>>2)];
            var c_cc = []
            for (var i = 0; i < 32; i++) {
              c_cc.push(HEAP8[(((argp + i)+(17))>>0)]);
            }
            return stream.tty.ops.ioctl_tcsets(stream.tty, op, { c_iflag, c_oflag, c_cflag, c_lflag, c_cc });
          }
          return 0; // no-op, not actually adjusting terminal settings
        }
        case 21519: {
          if (!stream.tty) return -59;
          var argp = SYSCALLS.get();
          HEAP32[((argp)>>2)] = 0;
          return 0;
        }
        case 21520: {
          if (!stream.tty) return -59;
          return -28; // not supported
        }
        case 21531: {
          var argp = SYSCALLS.get();
          return FS.ioctl(stream, op, argp);
        }
        case 21523: {
          // TODO: in theory we should write to the winsize struct that gets
          // passed in, but for now musl doesn't read anything on it
          if (!stream.tty) return -59;
          if (stream.tty.ops.ioctl_tiocgwinsz) {
            var winsize = stream.tty.ops.ioctl_tiocgwinsz(stream.tty);
            var argp = SYSCALLS.get();
            HEAP16[((argp)>>1)] = winsize[0];
            HEAP16[(((argp)+(2))>>1)] = winsize[1];
          }
          return 0;
        }
        case 21524: {
          // TODO: technically, this ioctl call should change the window size.
          // but, since emscripten doesn't have any concept of a terminal window
          // yet, we'll just silently throw it away as we do TIOCGWINSZ
          if (!stream.tty) return -59;
          return 0;
        }
        case 21515: {
          if (!stream.tty) return -59;
          return 0;
        }
        default: return -28; // not supported
      }
    } catch (e) {
    if (typeof FS == 'undefined' || !(e.name === 'ErrnoError')) throw e;
    return -e.errno;
  }
  }

  function ___syscall_lstat64(path, buf) {
  try {
  
      path = SYSCALLS.getStr(path);
      return SYSCALLS.doStat(FS.lstat, path, buf);
    } catch (e) {
    if (typeof FS == 'undefined' || !(e.name === 'ErrnoError')) throw e;
    return -e.errno;
  }
  }

  function ___syscall_newfstatat(dirfd, path, buf, flags) {
  try {
  
      path = SYSCALLS.getStr(path);
      var nofollow = flags & 256;
      var allowEmpty = flags & 4096;
      flags = flags & (~6400);
      assert(!flags, `unknown flags in __syscall_newfstatat: ${flags}`);
      path = SYSCALLS.calculateAt(dirfd, path, allowEmpty);
      return SYSCALLS.doStat(nofollow ? FS.lstat : FS.stat, path, buf);
    } catch (e) {
    if (typeof FS == 'undefined' || !(e.name === 'ErrnoError')) throw e;
    return -e.errno;
  }
  }

  function ___syscall_openat(dirfd, path, flags, varargs) {
  SYSCALLS.varargs = varargs;
  try {
  
      path = SYSCALLS.getStr(path);
      path = SYSCALLS.calculateAt(dirfd, path);
      var mode = varargs ? SYSCALLS.get() : 0;
      return FS.open(path, flags, mode).fd;
    } catch (e) {
    if (typeof FS == 'undefined' || !(e.name === 'ErrnoError')) throw e;
    return -e.errno;
  }
  }

  function ___syscall_stat64(path, buf) {
  try {
  
      path = SYSCALLS.getStr(path);
      return SYSCALLS.doStat(FS.stat, path, buf);
    } catch (e) {
    if (typeof FS == 'undefined' || !(e.name === 'ErrnoError')) throw e;
    return -e.errno;
  }
  }

  var nowIsMonotonic = true;;
  var __emscripten_get_now_is_monotonic = () => nowIsMonotonic;

  var __emscripten_throw_longjmp = () => {
      throw Infinity;
    };

  var _abort = () => {
      abort('native code called abort()');
    };

  function _emscripten_set_main_loop_timing(mode, value) {
      Browser.mainLoop.timingMode = mode;
      Browser.mainLoop.timingValue = value;
  
      if (!Browser.mainLoop.func) {
        err('emscripten_set_main_loop_timing: Cannot set timing mode for main loop since a main loop does not exist! Call emscripten_set_main_loop first to set one up.');
        return 1; // Return non-zero on failure, can't set timing mode when there is no main loop.
      }
  
      if (!Browser.mainLoop.running) {
        
        Browser.mainLoop.running = true;
      }
      if (mode == 0) {
        Browser.mainLoop.scheduler = function Browser_mainLoop_scheduler_setTimeout() {
          var timeUntilNextTick = Math.max(0, Browser.mainLoop.tickStartTime + value - _emscripten_get_now())|0;
          setTimeout(Browser.mainLoop.runner, timeUntilNextTick); // doing this each time means that on exception, we stop
        };
        Browser.mainLoop.method = 'timeout';
      } else if (mode == 1) {
        Browser.mainLoop.scheduler = function Browser_mainLoop_scheduler_rAF() {
          Browser.requestAnimationFrame(Browser.mainLoop.runner);
        };
        Browser.mainLoop.method = 'rAF';
      } else if (mode == 2) {
        if (typeof setImmediate == 'undefined') {
          // Emulate setImmediate. (note: not a complete polyfill, we don't emulate clearImmediate() to keep code size to minimum, since not needed)
          var setImmediates = [];
          var emscriptenMainLoopMessageId = 'setimmediate';
          /** @param {Event} event */
          var Browser_setImmediate_messageHandler = (event) => {
            // When called in current thread or Worker, the main loop ID is structured slightly different to accommodate for --proxy-to-worker runtime listening to Worker events,
            // so check for both cases.
            if (event.data === emscriptenMainLoopMessageId || event.data.target === emscriptenMainLoopMessageId) {
              event.stopPropagation();
              setImmediates.shift()();
            }
          };
          addEventListener("message", Browser_setImmediate_messageHandler, true);
          setImmediate = /** @type{function(function(): ?, ...?): number} */(function Browser_emulated_setImmediate(func) {
            setImmediates.push(func);
            if (ENVIRONMENT_IS_WORKER) {
              if (Module['setImmediates'] === undefined) Module['setImmediates'] = [];
              Module['setImmediates'].push(func);
              postMessage({target: emscriptenMainLoopMessageId}); // In --proxy-to-worker, route the message via proxyClient.js
            } else postMessage(emscriptenMainLoopMessageId, "*"); // On the main thread, can just send the message to itself.
          })
        }
        Browser.mainLoop.scheduler = function Browser_mainLoop_scheduler_setImmediate() {
          setImmediate(Browser.mainLoop.runner);
        };
        Browser.mainLoop.method = 'immediate';
      }
      return 0;
    }
  
  var _emscripten_get_now;
      // Modern environment where performance.now() is supported:
      // N.B. a shorter form "_emscripten_get_now = performance.now;" is
      // unfortunately not allowed even in current browsers (e.g. FF Nightly 75).
      _emscripten_get_now = () => performance.now();
  ;
  
  
    /**
     * @param {number=} arg
     * @param {boolean=} noSetTiming
     */
  function setMainLoop(browserIterationFunc, fps, simulateInfiniteLoop, arg, noSetTiming) {
      assert(!Browser.mainLoop.func, 'emscripten_set_main_loop: there can only be one main loop function at once: call emscripten_cancel_main_loop to cancel the previous one before setting a new one with different parameters.');
  
      Browser.mainLoop.func = browserIterationFunc;
      Browser.mainLoop.arg = arg;
  
      var thisMainLoopId = Browser.mainLoop.currentlyRunningMainloop;
      function checkIsRunning() {
        if (thisMainLoopId < Browser.mainLoop.currentlyRunningMainloop) {
          
          return false;
        }
        return true;
      }
  
      // We create the loop runner here but it is not actually running until
      // _emscripten_set_main_loop_timing is called (which might happen a
      // later time).  This member signifies that the current runner has not
      // yet been started so that we can call runtimeKeepalivePush when it
      // gets it timing set for the first time.
      Browser.mainLoop.running = false;
      Browser.mainLoop.runner = function Browser_mainLoop_runner() {
        if (ABORT) return;
        if (Browser.mainLoop.queue.length > 0) {
          var start = Date.now();
          var blocker = Browser.mainLoop.queue.shift();
          blocker.func(blocker.arg);
          if (Browser.mainLoop.remainingBlockers) {
            var remaining = Browser.mainLoop.remainingBlockers;
            var next = remaining%1 == 0 ? remaining-1 : Math.floor(remaining);
            if (blocker.counted) {
              Browser.mainLoop.remainingBlockers = next;
            } else {
              // not counted, but move the progress along a tiny bit
              next = next + 0.5; // do not steal all the next one's progress
              Browser.mainLoop.remainingBlockers = (8*remaining + next)/9;
            }
          }
          out('main loop blocker "' + blocker.name + '" took ' + (Date.now() - start) + ' ms'); //, left: ' + Browser.mainLoop.remainingBlockers);
          Browser.mainLoop.updateStatus();
  
          // catches pause/resume main loop from blocker execution
          if (!checkIsRunning()) return;
  
          setTimeout(Browser.mainLoop.runner, 0);
          return;
        }
  
        // catch pauses from non-main loop sources
        if (!checkIsRunning()) return;
  
        // Implement very basic swap interval control
        Browser.mainLoop.currentFrameNumber = Browser.mainLoop.currentFrameNumber + 1 | 0;
        if (Browser.mainLoop.timingMode == 1 && Browser.mainLoop.timingValue > 1 && Browser.mainLoop.currentFrameNumber % Browser.mainLoop.timingValue != 0) {
          // Not the scheduled time to render this frame - skip.
          Browser.mainLoop.scheduler();
          return;
        } else if (Browser.mainLoop.timingMode == 0) {
          Browser.mainLoop.tickStartTime = _emscripten_get_now();
        }
  
        // Signal GL rendering layer that processing of a new frame is about to start. This helps it optimize
        // VBO double-buffering and reduce GPU stalls.
        GL.newRenderingFrameStarted();
  
        if (Browser.mainLoop.method === 'timeout' && Module.ctx) {
          warnOnce('Looks like you are rendering without using requestAnimationFrame for the main loop. You should use 0 for the frame rate in emscripten_set_main_loop in order to use requestAnimationFrame, as that can greatly improve your frame rates!');
          Browser.mainLoop.method = ''; // just warn once per call to set main loop
        }
  
        Browser.mainLoop.runIter(browserIterationFunc);
  
        checkStackCookie();
  
        // catch pauses from the main loop itself
        if (!checkIsRunning()) return;
  
        // Queue new audio data. This is important to be right after the main loop invocation, so that we will immediately be able
        // to queue the newest produced audio samples.
        // TODO: Consider adding pre- and post- rAF callbacks so that GL.newRenderingFrameStarted() and SDL.audio.queueNewAudioData()
        //       do not need to be hardcoded into this function, but can be more generic.
        if (typeof SDL == 'object' && SDL.audio && SDL.audio.queueNewAudioData) SDL.audio.queueNewAudioData();
  
        Browser.mainLoop.scheduler();
      }
  
      if (!noSetTiming) {
        if (fps && fps > 0) {
          _emscripten_set_main_loop_timing(0, 1000.0 / fps);
        } else {
          // Do rAF by rendering each frame (no decimating)
          _emscripten_set_main_loop_timing(1, 1);
        }
  
        Browser.mainLoop.scheduler();
      }
  
      if (simulateInfiniteLoop) {
        throw 'unwind';
      }
    }
  
  var handleException = (e) => {
      // Certain exception types we do not treat as errors since they are used for
      // internal control flow.
      // 1. ExitStatus, which is thrown by exit()
      // 2. "unwind", which is thrown by emscripten_unwind_to_js_event_loop() and others
      //    that wish to return to JS event loop.
      if (e instanceof ExitStatus || e == 'unwind') {
        return EXITSTATUS;
      }
      checkStackCookie();
      if (e instanceof WebAssembly.RuntimeError) {
        if (_emscripten_stack_get_current() <= 0) {
          err('Stack overflow detected.  You can try increasing -sSTACK_SIZE (currently set to 65536)');
        }
      }
      quit_(1, e);
    };
  
  
  var _proc_exit = (code) => {
      EXITSTATUS = code;
      if (!keepRuntimeAlive()) {
        if (Module['onExit']) Module['onExit'](code);
        ABORT = true;
      }
      quit_(code, new ExitStatus(code));
    };
  /** @suppress {duplicate } */
  /** @param {boolean|number=} implicit */
  var exitJS = (status, implicit) => {
      EXITSTATUS = status;
  
      checkUnflushedContent();
  
      // if exit() was called explicitly, warn the user if the runtime isn't actually being shut down
      if (keepRuntimeAlive() && !implicit) {
        var msg = `program exited (with status: ${status}), but keepRuntimeAlive() is set (counter=${runtimeKeepaliveCounter}) due to an async operation, so halting execution but not exiting the runtime or preventing further async execution (you can use emscripten_force_exit, if you want to force a true shutdown)`;
        err(msg);
      }
  
      _proc_exit(status);
    };
  var _exit = exitJS;
  
  var maybeExit = () => {
      if (!keepRuntimeAlive()) {
        try {
          _exit(EXITSTATUS);
        } catch (e) {
          handleException(e);
        }
      }
    };
  var callUserCallback = (func) => {
      if (ABORT) {
        err('user callback triggered after runtime exited or application aborted.  Ignoring.');
        return;
      }
      try {
        func();
        maybeExit();
      } catch (e) {
        handleException(e);
      }
    };
  
  /** @param {number=} timeout */
  var safeSetTimeout = (func, timeout) => {
      
      return setTimeout(() => {
        
        callUserCallback(func);
      }, timeout);
    };
  
  
  
  
  var Browser = {
  mainLoop:{
  running:false,
  scheduler:null,
  method:"",
  currentlyRunningMainloop:0,
  func:null,
  arg:0,
  timingMode:0,
  timingValue:0,
  currentFrameNumber:0,
  queue:[],
  pause:function() {
          Browser.mainLoop.scheduler = null;
          // Incrementing this signals the previous main loop that it's now become old, and it must return.
          Browser.mainLoop.currentlyRunningMainloop++;
        },
  resume:function() {
          Browser.mainLoop.currentlyRunningMainloop++;
          var timingMode = Browser.mainLoop.timingMode;
          var timingValue = Browser.mainLoop.timingValue;
          var func = Browser.mainLoop.func;
          Browser.mainLoop.func = null;
          // do not set timing and call scheduler, we will do it on the next lines
          setMainLoop(func, 0, false, Browser.mainLoop.arg, true);
          _emscripten_set_main_loop_timing(timingMode, timingValue);
          Browser.mainLoop.scheduler();
        },
  updateStatus:function() {
          if (Module['setStatus']) {
            var message = Module['statusMessage'] || 'Please wait...';
            var remaining = Browser.mainLoop.remainingBlockers;
            var expected = Browser.mainLoop.expectedBlockers;
            if (remaining) {
              if (remaining < expected) {
                Module['setStatus'](message + ' (' + (expected - remaining) + '/' + expected + ')');
              } else {
                Module['setStatus'](message);
              }
            } else {
              Module['setStatus']('');
            }
          }
        },
  runIter:function(func) {
          if (ABORT) return;
          if (Module['preMainLoop']) {
            var preRet = Module['preMainLoop']();
            if (preRet === false) {
              return; // |return false| skips a frame
            }
          }
          callUserCallback(func);
          if (Module['postMainLoop']) Module['postMainLoop']();
        },
  },
  isFullscreen:false,
  pointerLock:false,
  moduleContextCreatedCallbacks:[],
  workers:[],
  init:function() {
        if (Browser.initted) return;
        Browser.initted = true;
  
        // Support for plugins that can process preloaded files. You can add more of these to
        // your app by creating and appending to preloadPlugins.
        //
        // Each plugin is asked if it can handle a file based on the file's name. If it can,
        // it is given the file's raw data. When it is done, it calls a callback with the file's
        // (possibly modified) data. For example, a plugin might decompress a file, or it
        // might create some side data structure for use later (like an Image element, etc.).
  
        var imagePlugin = {};
        imagePlugin['canHandle'] = function imagePlugin_canHandle(name) {
          return !Module.noImageDecoding && /\.(jpg|jpeg|png|bmp)$/i.test(name);
        };
        imagePlugin['handle'] = function imagePlugin_handle(byteArray, name, onload, onerror) {
          var b = new Blob([byteArray], { type: Browser.getMimetype(name) });
          if (b.size !== byteArray.length) { // Safari bug #118630
            // Safari's Blob can only take an ArrayBuffer
            b = new Blob([(new Uint8Array(byteArray)).buffer], { type: Browser.getMimetype(name) });
          }
          var url = URL.createObjectURL(b);
          assert(typeof url == 'string', 'createObjectURL must return a url as a string');
          var img = new Image();
          img.onload = () => {
            assert(img.complete, 'Image ' + name + ' could not be decoded');
            var canvas = /** @type {!HTMLCanvasElement} */ (document.createElement('canvas'));
            canvas.width = img.width;
            canvas.height = img.height;
            var ctx = canvas.getContext('2d');
            ctx.drawImage(img, 0, 0);
            preloadedImages[name] = canvas;
            URL.revokeObjectURL(url);
            if (onload) onload(byteArray);
          };
          img.onerror = (event) => {
            out('Image ' + url + ' could not be decoded');
            if (onerror) onerror();
          };
          img.src = url;
        };
        preloadPlugins.push(imagePlugin);
  
        var audioPlugin = {};
        audioPlugin['canHandle'] = function audioPlugin_canHandle(name) {
          return !Module.noAudioDecoding && name.substr(-4) in { '.ogg': 1, '.wav': 1, '.mp3': 1 };
        };
        audioPlugin['handle'] = function audioPlugin_handle(byteArray, name, onload, onerror) {
          var done = false;
          function finish(audio) {
            if (done) return;
            done = true;
            preloadedAudios[name] = audio;
            if (onload) onload(byteArray);
          }
          function fail() {
            if (done) return;
            done = true;
            preloadedAudios[name] = new Audio(); // empty shim
            if (onerror) onerror();
          }
          var b = new Blob([byteArray], { type: Browser.getMimetype(name) });
          var url = URL.createObjectURL(b); // XXX we never revoke this!
          assert(typeof url == 'string', 'createObjectURL must return a url as a string');
          var audio = new Audio();
          audio.addEventListener('canplaythrough', () => finish(audio), false); // use addEventListener due to chromium bug 124926
          audio.onerror = function audio_onerror(event) {
            if (done) return;
            err('warning: browser could not fully decode audio ' + name + ', trying slower base64 approach');
            function encode64(data) {
              var BASE = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/';
              var PAD = '=';
              var ret = '';
              var leftchar = 0;
              var leftbits = 0;
              for (var i = 0; i < data.length; i++) {
                leftchar = (leftchar << 8) | data[i];
                leftbits += 8;
                while (leftbits >= 6) {
                  var curr = (leftchar >> (leftbits-6)) & 0x3f;
                  leftbits -= 6;
                  ret += BASE[curr];
                }
              }
              if (leftbits == 2) {
                ret += BASE[(leftchar&3) << 4];
                ret += PAD + PAD;
              } else if (leftbits == 4) {
                ret += BASE[(leftchar&0xf) << 2];
                ret += PAD;
              }
              return ret;
            }
            audio.src = 'data:audio/x-' + name.substr(-3) + ';base64,' + encode64(byteArray);
            finish(audio); // we don't wait for confirmation this worked - but it's worth trying
          };
          audio.src = url;
          // workaround for chrome bug 124926 - we do not always get oncanplaythrough or onerror
          safeSetTimeout(() => {
            finish(audio); // try to use it even though it is not necessarily ready to play
          }, 10000);
        };
        preloadPlugins.push(audioPlugin);
  
        // Canvas event setup
  
        function pointerLockChange() {
          Browser.pointerLock = document['pointerLockElement'] === Module['canvas'] ||
                                document['mozPointerLockElement'] === Module['canvas'] ||
                                document['webkitPointerLockElement'] === Module['canvas'] ||
                                document['msPointerLockElement'] === Module['canvas'];
        }
        var canvas = Module['canvas'];
        if (canvas) {
          // forced aspect ratio can be enabled by defining 'forcedAspectRatio' on Module
          // Module['forcedAspectRatio'] = 4 / 3;
  
          canvas.requestPointerLock = canvas['requestPointerLock'] ||
                                      canvas['mozRequestPointerLock'] ||
                                      canvas['webkitRequestPointerLock'] ||
                                      canvas['msRequestPointerLock'] ||
                                      (() => {});
          canvas.exitPointerLock = document['exitPointerLock'] ||
                                   document['mozExitPointerLock'] ||
                                   document['webkitExitPointerLock'] ||
                                   document['msExitPointerLock'] ||
                                   (() => {}); // no-op if function does not exist
          canvas.exitPointerLock = canvas.exitPointerLock.bind(document);
  
          document.addEventListener('pointerlockchange', pointerLockChange, false);
          document.addEventListener('mozpointerlockchange', pointerLockChange, false);
          document.addEventListener('webkitpointerlockchange', pointerLockChange, false);
          document.addEventListener('mspointerlockchange', pointerLockChange, false);
  
          if (Module['elementPointerLock']) {
            canvas.addEventListener("click", (ev) => {
              if (!Browser.pointerLock && Module['canvas'].requestPointerLock) {
                Module['canvas'].requestPointerLock();
                ev.preventDefault();
              }
            }, false);
          }
        }
      },
  createContext:function(/** @type {HTMLCanvasElement} */ canvas, useWebGL, setInModule, webGLContextAttributes) {
        if (useWebGL && Module.ctx && canvas == Module.canvas) return Module.ctx; // no need to recreate GL context if it's already been created for this canvas.
  
        var ctx;
        var contextHandle;
        if (useWebGL) {
          // For GLES2/desktop GL compatibility, adjust a few defaults to be different to WebGL defaults, so that they align better with the desktop defaults.
          var contextAttributes = {
            antialias: false,
            alpha: false,
            majorVersion: (typeof WebGL2RenderingContext != 'undefined') ? 2 : 1,
          };
  
          if (webGLContextAttributes) {
            for (var attribute in webGLContextAttributes) {
              contextAttributes[attribute] = webGLContextAttributes[attribute];
            }
          }
  
          // This check of existence of GL is here to satisfy Closure compiler, which yells if variable GL is referenced below but GL object is not
          // actually compiled in because application is not doing any GL operations. TODO: Ideally if GL is not being used, this function
          // Browser.createContext() should not even be emitted.
          if (typeof GL != 'undefined') {
            contextHandle = GL.createContext(canvas, contextAttributes);
            if (contextHandle) {
              ctx = GL.getContext(contextHandle).GLctx;
            }
          }
        } else {
          ctx = canvas.getContext('2d');
        }
  
        if (!ctx) return null;
  
        if (setInModule) {
          if (!useWebGL) assert(typeof GLctx == 'undefined', 'cannot set in module if GLctx is used, but we are a non-GL context that would replace it');
  
          Module.ctx = ctx;
          if (useWebGL) GL.makeContextCurrent(contextHandle);
          Module.useWebGL = useWebGL;
          Browser.moduleContextCreatedCallbacks.forEach((callback) => callback());
          Browser.init();
        }
        return ctx;
      },
  destroyContext:function(canvas, useWebGL, setInModule) {},
  fullscreenHandlersInstalled:false,
  lockPointer:undefined,
  resizeCanvas:undefined,
  requestFullscreen:function(lockPointer, resizeCanvas) {
        Browser.lockPointer = lockPointer;
        Browser.resizeCanvas = resizeCanvas;
        if (typeof Browser.lockPointer == 'undefined') Browser.lockPointer = true;
        if (typeof Browser.resizeCanvas == 'undefined') Browser.resizeCanvas = false;
  
        var canvas = Module['canvas'];
        function fullscreenChange() {
          Browser.isFullscreen = false;
          var canvasContainer = canvas.parentNode;
          if ((document['fullscreenElement'] || document['mozFullScreenElement'] ||
               document['msFullscreenElement'] || document['webkitFullscreenElement'] ||
               document['webkitCurrentFullScreenElement']) === canvasContainer) {
            canvas.exitFullscreen = Browser.exitFullscreen;
            if (Browser.lockPointer) canvas.requestPointerLock();
            Browser.isFullscreen = true;
            if (Browser.resizeCanvas) {
              Browser.setFullscreenCanvasSize();
            } else {
              Browser.updateCanvasDimensions(canvas);
            }
          } else {
            // remove the full screen specific parent of the canvas again to restore the HTML structure from before going full screen
            canvasContainer.parentNode.insertBefore(canvas, canvasContainer);
            canvasContainer.parentNode.removeChild(canvasContainer);
  
            if (Browser.resizeCanvas) {
              Browser.setWindowedCanvasSize();
            } else {
              Browser.updateCanvasDimensions(canvas);
            }
          }
          if (Module['onFullScreen']) Module['onFullScreen'](Browser.isFullscreen);
          if (Module['onFullscreen']) Module['onFullscreen'](Browser.isFullscreen);
        }
  
        if (!Browser.fullscreenHandlersInstalled) {
          Browser.fullscreenHandlersInstalled = true;
          document.addEventListener('fullscreenchange', fullscreenChange, false);
          document.addEventListener('mozfullscreenchange', fullscreenChange, false);
          document.addEventListener('webkitfullscreenchange', fullscreenChange, false);
          document.addEventListener('MSFullscreenChange', fullscreenChange, false);
        }
  
        // create a new parent to ensure the canvas has no siblings. this allows browsers to optimize full screen performance when its parent is the full screen root
        var canvasContainer = document.createElement("div");
        canvas.parentNode.insertBefore(canvasContainer, canvas);
        canvasContainer.appendChild(canvas);
  
        // use parent of canvas as full screen root to allow aspect ratio correction (Firefox stretches the root to screen size)
        canvasContainer.requestFullscreen = canvasContainer['requestFullscreen'] ||
                                            canvasContainer['mozRequestFullScreen'] ||
                                            canvasContainer['msRequestFullscreen'] ||
                                           (canvasContainer['webkitRequestFullscreen'] ? () => canvasContainer['webkitRequestFullscreen'](Element['ALLOW_KEYBOARD_INPUT']) : null) ||
                                           (canvasContainer['webkitRequestFullScreen'] ? () => canvasContainer['webkitRequestFullScreen'](Element['ALLOW_KEYBOARD_INPUT']) : null);
  
        canvasContainer.requestFullscreen();
      },
  requestFullScreen:function() {
        abort('Module.requestFullScreen has been replaced by Module.requestFullscreen (without a capital S)');
      },
  exitFullscreen:function() {
        // This is workaround for chrome. Trying to exit from fullscreen
        // not in fullscreen state will cause "TypeError: Document not active"
        // in chrome. See https://github.com/emscripten-core/emscripten/pull/8236
        if (!Browser.isFullscreen) {
          return false;
        }
  
        var CFS = document['exitFullscreen'] ||
                  document['cancelFullScreen'] ||
                  document['mozCancelFullScreen'] ||
                  document['msExitFullscreen'] ||
                  document['webkitCancelFullScreen'] ||
            (() => {});
        CFS.apply(document, []);
        return true;
      },
  nextRAF:0,
  fakeRequestAnimationFrame:function(func) {
        // try to keep 60fps between calls to here
        var now = Date.now();
        if (Browser.nextRAF === 0) {
          Browser.nextRAF = now + 1000/60;
        } else {
          while (now + 2 >= Browser.nextRAF) { // fudge a little, to avoid timer jitter causing us to do lots of delay:0
            Browser.nextRAF += 1000/60;
          }
        }
        var delay = Math.max(Browser.nextRAF - now, 0);
        setTimeout(func, delay);
      },
  requestAnimationFrame:function(func) {
        if (typeof requestAnimationFrame == 'function') {
          requestAnimationFrame(func);
          return;
        }
        var RAF = Browser.fakeRequestAnimationFrame;
        RAF(func);
      },
  safeSetTimeout:function(func, timeout) {
        // Legacy function, this is used by the SDL2 port so we need to keep it
        // around at least until that is updated.
        // See https://github.com/libsdl-org/SDL/pull/6304
        return safeSetTimeout(func, timeout);
      },
  safeRequestAnimationFrame:function(func) {
        
        return Browser.requestAnimationFrame(() => {
          
          callUserCallback(func);
        });
      },
  getMimetype:function(name) {
        return {
          'jpg': 'image/jpeg',
          'jpeg': 'image/jpeg',
          'png': 'image/png',
          'bmp': 'image/bmp',
          'ogg': 'audio/ogg',
          'wav': 'audio/wav',
          'mp3': 'audio/mpeg'
        }[name.substr(name.lastIndexOf('.')+1)];
      },
  getUserMedia:function(func) {
        if (!window.getUserMedia) {
          window.getUserMedia = navigator['getUserMedia'] ||
                                navigator['mozGetUserMedia'];
        }
        window.getUserMedia(func);
      },
  getMovementX:function(event) {
        return event['movementX'] ||
               event['mozMovementX'] ||
               event['webkitMovementX'] ||
               0;
      },
  getMovementY:function(event) {
        return event['movementY'] ||
               event['mozMovementY'] ||
               event['webkitMovementY'] ||
               0;
      },
  getMouseWheelDelta:function(event) {
        var delta = 0;
        switch (event.type) {
          case 'DOMMouseScroll':
            // 3 lines make up a step
            delta = event.detail / 3;
            break;
          case 'mousewheel':
            // 120 units make up a step
            delta = event.wheelDelta / 120;
            break;
          case 'wheel':
            delta = event.deltaY
            switch (event.deltaMode) {
              case 0:
                // DOM_DELTA_PIXEL: 100 pixels make up a step
                delta /= 100;
                break;
              case 1:
                // DOM_DELTA_LINE: 3 lines make up a step
                delta /= 3;
                break;
              case 2:
                // DOM_DELTA_PAGE: A page makes up 80 steps
                delta *= 80;
                break;
              default:
                throw 'unrecognized mouse wheel delta mode: ' + event.deltaMode;
            }
            break;
          default:
            throw 'unrecognized mouse wheel event: ' + event.type;
        }
        return delta;
      },
  mouseX:0,
  mouseY:0,
  mouseMovementX:0,
  mouseMovementY:0,
  touches:{
  },
  lastTouches:{
  },
  calculateMouseEvent:function(event) { // event should be mousemove, mousedown or mouseup
        if (Browser.pointerLock) {
          // When the pointer is locked, calculate the coordinates
          // based on the movement of the mouse.
          // Workaround for Firefox bug 764498
          if (event.type != 'mousemove' &&
              ('mozMovementX' in event)) {
            Browser.mouseMovementX = Browser.mouseMovementY = 0;
          } else {
            Browser.mouseMovementX = Browser.getMovementX(event);
            Browser.mouseMovementY = Browser.getMovementY(event);
          }
  
          // check if SDL is available
          if (typeof SDL != "undefined") {
            Browser.mouseX = SDL.mouseX + Browser.mouseMovementX;
            Browser.mouseY = SDL.mouseY + Browser.mouseMovementY;
          } else {
            // just add the mouse delta to the current absolut mouse position
            // FIXME: ideally this should be clamped against the canvas size and zero
            Browser.mouseX += Browser.mouseMovementX;
            Browser.mouseY += Browser.mouseMovementY;
          }
        } else {
          // Otherwise, calculate the movement based on the changes
          // in the coordinates.
          var rect = Module["canvas"].getBoundingClientRect();
          var cw = Module["canvas"].width;
          var ch = Module["canvas"].height;
  
          // Neither .scrollX or .pageXOffset are defined in a spec, but
          // we prefer .scrollX because it is currently in a spec draft.
          // (see: http://www.w3.org/TR/2013/WD-cssom-view-20131217/)
          var scrollX = ((typeof window.scrollX != 'undefined') ? window.scrollX : window.pageXOffset);
          var scrollY = ((typeof window.scrollY != 'undefined') ? window.scrollY : window.pageYOffset);
          // If this assert lands, it's likely because the browser doesn't support scrollX or pageXOffset
          // and we have no viable fallback.
          assert((typeof scrollX != 'undefined') && (typeof scrollY != 'undefined'), 'Unable to retrieve scroll position, mouse positions likely broken.');
  
          if (event.type === 'touchstart' || event.type === 'touchend' || event.type === 'touchmove') {
            var touch = event.touch;
            if (touch === undefined) {
              return; // the "touch" property is only defined in SDL
  
            }
            var adjustedX = touch.pageX - (scrollX + rect.left);
            var adjustedY = touch.pageY - (scrollY + rect.top);
  
            adjustedX = adjustedX * (cw / rect.width);
            adjustedY = adjustedY * (ch / rect.height);
  
            var coords = { x: adjustedX, y: adjustedY };
  
            if (event.type === 'touchstart') {
              Browser.lastTouches[touch.identifier] = coords;
              Browser.touches[touch.identifier] = coords;
            } else if (event.type === 'touchend' || event.type === 'touchmove') {
              var last = Browser.touches[touch.identifier];
              if (!last) last = coords;
              Browser.lastTouches[touch.identifier] = last;
              Browser.touches[touch.identifier] = coords;
            }
            return;
          }
  
          var x = event.pageX - (scrollX + rect.left);
          var y = event.pageY - (scrollY + rect.top);
  
          // the canvas might be CSS-scaled compared to its backbuffer;
          // SDL-using content will want mouse coordinates in terms
          // of backbuffer units.
          x = x * (cw / rect.width);
          y = y * (ch / rect.height);
  
          Browser.mouseMovementX = x - Browser.mouseX;
          Browser.mouseMovementY = y - Browser.mouseY;
          Browser.mouseX = x;
          Browser.mouseY = y;
        }
      },
  resizeListeners:[],
  updateResizeListeners:function() {
        var canvas = Module['canvas'];
        Browser.resizeListeners.forEach((listener) => listener(canvas.width, canvas.height));
      },
  setCanvasSize:function(width, height, noUpdates) {
        var canvas = Module['canvas'];
        Browser.updateCanvasDimensions(canvas, width, height);
        if (!noUpdates) Browser.updateResizeListeners();
      },
  windowedWidth:0,
  windowedHeight:0,
  setFullscreenCanvasSize:function() {
        // check if SDL is available
        if (typeof SDL != "undefined") {
          var flags = HEAPU32[((SDL.screen)>>2)];
          flags = flags | 0x00800000; // set SDL_FULLSCREEN flag
          HEAP32[((SDL.screen)>>2)] = flags;
        }
        Browser.updateCanvasDimensions(Module['canvas']);
        Browser.updateResizeListeners();
      },
  setWindowedCanvasSize:function() {
        // check if SDL is available
        if (typeof SDL != "undefined") {
          var flags = HEAPU32[((SDL.screen)>>2)];
          flags = flags & ~0x00800000; // clear SDL_FULLSCREEN flag
          HEAP32[((SDL.screen)>>2)] = flags;
        }
        Browser.updateCanvasDimensions(Module['canvas']);
        Browser.updateResizeListeners();
      },
  updateCanvasDimensions:function(canvas, wNative, hNative) {
        if (wNative && hNative) {
          canvas.widthNative = wNative;
          canvas.heightNative = hNative;
        } else {
          wNative = canvas.widthNative;
          hNative = canvas.heightNative;
        }
        var w = wNative;
        var h = hNative;
        if (Module['forcedAspectRatio'] && Module['forcedAspectRatio'] > 0) {
          if (w/h < Module['forcedAspectRatio']) {
            w = Math.round(h * Module['forcedAspectRatio']);
          } else {
            h = Math.round(w / Module['forcedAspectRatio']);
          }
        }
        if (((document['fullscreenElement'] || document['mozFullScreenElement'] ||
             document['msFullscreenElement'] || document['webkitFullscreenElement'] ||
             document['webkitCurrentFullScreenElement']) === canvas.parentNode) && (typeof screen != 'undefined')) {
           var factor = Math.min(screen.width / w, screen.height / h);
           w = Math.round(w * factor);
           h = Math.round(h * factor);
        }
        if (Browser.resizeCanvas) {
          if (canvas.width  != w) canvas.width  = w;
          if (canvas.height != h) canvas.height = h;
          if (typeof canvas.style != 'undefined') {
            canvas.style.removeProperty( "width");
            canvas.style.removeProperty("height");
          }
        } else {
          if (canvas.width  != wNative) canvas.width  = wNative;
          if (canvas.height != hNative) canvas.height = hNative;
          if (typeof canvas.style != 'undefined') {
            if (w != wNative || h != hNative) {
              canvas.style.setProperty( "width", w + "px", "important");
              canvas.style.setProperty("height", h + "px", "important");
            } else {
              canvas.style.removeProperty( "width");
              canvas.style.removeProperty("height");
            }
          }
        }
      },
  };
  
  var EGL = {
  errorCode:12288,
  defaultDisplayInitialized:false,
  currentContext:0,
  currentReadSurface:0,
  currentDrawSurface:0,
  contextAttributes:{
  alpha:false,
  depth:false,
  stencil:false,
  antialias:false,
  },
  stringCache:{
  },
  setErrorCode:function(code) {
        EGL.errorCode = code;
      },
  chooseConfig:function(display, attribList, config, config_size, numConfigs) {
        if (display != 62000 /* Magic ID for Emscripten 'default display' */) {
          EGL.setErrorCode(0x3008 /* EGL_BAD_DISPLAY */);
          return 0;
        }
  
        if (attribList) {
          // read attribList if it is non-null
          for (;;) {
            var param = HEAP32[((attribList)>>2)];
            if (param == 0x3021 /*EGL_ALPHA_SIZE*/) {
              var alphaSize = HEAP32[(((attribList)+(4))>>2)];
              EGL.contextAttributes.alpha = (alphaSize > 0);
            } else if (param == 0x3025 /*EGL_DEPTH_SIZE*/) {
              var depthSize = HEAP32[(((attribList)+(4))>>2)];
              EGL.contextAttributes.depth = (depthSize > 0);
            } else if (param == 0x3026 /*EGL_STENCIL_SIZE*/) {
              var stencilSize = HEAP32[(((attribList)+(4))>>2)];
              EGL.contextAttributes.stencil = (stencilSize > 0);
            } else if (param == 0x3031 /*EGL_SAMPLES*/) {
              var samples = HEAP32[(((attribList)+(4))>>2)];
              EGL.contextAttributes.antialias = (samples > 0);
            } else if (param == 0x3032 /*EGL_SAMPLE_BUFFERS*/) {
              var samples = HEAP32[(((attribList)+(4))>>2)];
              EGL.contextAttributes.antialias = (samples == 1);
            } else if (param == 0x3100 /*EGL_CONTEXT_PRIORITY_LEVEL_IMG*/) {
              var requestedPriority = HEAP32[(((attribList)+(4))>>2)];
              EGL.contextAttributes.lowLatency = (requestedPriority != 0x3103 /*EGL_CONTEXT_PRIORITY_LOW_IMG*/);
            } else if (param == 0x3038 /*EGL_NONE*/) {
                break;
            }
            attribList += 8;
          }
        }
  
        if ((!config || !config_size) && !numConfigs) {
          EGL.setErrorCode(0x300C /* EGL_BAD_PARAMETER */);
          return 0;
        }
        if (numConfigs) {
          HEAP32[((numConfigs)>>2)] = 1; // Total number of supported configs: 1.
        }
        if (config && config_size > 0) {
          HEAP32[((config)>>2)] = 62002;
        }
  
        EGL.setErrorCode(0x3000 /* EGL_SUCCESS */);
        return 1;
      },
  };
  function _eglBindAPI(api) {
      if (api == 0x30A0 /* EGL_OPENGL_ES_API */) {
        EGL.setErrorCode(0x3000 /* EGL_SUCCESS */);
        return 1;
      }
      // if (api == 0x30A1 /* EGL_OPENVG_API */ || api == 0x30A2 /* EGL_OPENGL_API */) {
      EGL.setErrorCode(0x300C /* EGL_BAD_PARAMETER */);
      return 0;
    }

  function _eglChooseConfig(display, attrib_list, configs, config_size, numConfigs) {
      return EGL.chooseConfig(display, attrib_list, configs, config_size, numConfigs);
    }

  function webgl_enable_ANGLE_instanced_arrays(ctx) {
      // Extension available in WebGL 1 from Firefox 26 and Google Chrome 30 onwards. Core feature in WebGL 2.
      var ext = ctx.getExtension('ANGLE_instanced_arrays');
      if (ext) {
        ctx['vertexAttribDivisor'] = function(index, divisor) { ext['vertexAttribDivisorANGLE'](index, divisor); };
        ctx['drawArraysInstanced'] = function(mode, first, count, primcount) { ext['drawArraysInstancedANGLE'](mode, first, count, primcount); };
        ctx['drawElementsInstanced'] = function(mode, count, type, indices, primcount) { ext['drawElementsInstancedANGLE'](mode, count, type, indices, primcount); };
        return 1;
      }
    }
  
  function webgl_enable_OES_vertex_array_object(ctx) {
      // Extension available in WebGL 1 from Firefox 25 and WebKit 536.28/desktop Safari 6.0.3 onwards. Core feature in WebGL 2.
      var ext = ctx.getExtension('OES_vertex_array_object');
      if (ext) {
        ctx['createVertexArray'] = function() { return ext['createVertexArrayOES'](); };
        ctx['deleteVertexArray'] = function(vao) { ext['deleteVertexArrayOES'](vao); };
        ctx['bindVertexArray'] = function(vao) { ext['bindVertexArrayOES'](vao); };
        ctx['isVertexArray'] = function(vao) { return ext['isVertexArrayOES'](vao); };
        return 1;
      }
    }
  
  function webgl_enable_WEBGL_draw_buffers(ctx) {
      // Extension available in WebGL 1 from Firefox 28 onwards. Core feature in WebGL 2.
      var ext = ctx.getExtension('WEBGL_draw_buffers');
      if (ext) {
        ctx['drawBuffers'] = function(n, bufs) { ext['drawBuffersWEBGL'](n, bufs); };
        return 1;
      }
    }
  
  function webgl_enable_WEBGL_draw_instanced_base_vertex_base_instance(ctx) {
      // Closure is expected to be allowed to minify the '.dibvbi' property, so not accessing it quoted.
      return !!(ctx.dibvbi = ctx.getExtension('WEBGL_draw_instanced_base_vertex_base_instance'));
    }
  
  function webgl_enable_WEBGL_multi_draw_instanced_base_vertex_base_instance(ctx) {
      // Closure is expected to be allowed to minify the '.mdibvbi' property, so not accessing it quoted.
      return !!(ctx.mdibvbi = ctx.getExtension('WEBGL_multi_draw_instanced_base_vertex_base_instance'));
    }
  
  function webgl_enable_WEBGL_multi_draw(ctx) {
      // Closure is expected to be allowed to minify the '.multiDrawWebgl' property, so not accessing it quoted.
      return !!(ctx.multiDrawWebgl = ctx.getExtension('WEBGL_multi_draw'));
    }
  
  
  var GL = {
  counter:1,
  buffers:[],
  mappedBuffers:{
  },
  programs:[],
  framebuffers:[],
  renderbuffers:[],
  textures:[],
  shaders:[],
  vaos:[],
  contexts:[],
  offscreenCanvases:{
  },
  queries:[],
  samplers:[],
  transformFeedbacks:[],
  syncs:[],
  byteSizeByTypeRoot:5120,
  byteSizeByType:[1,1,2,2,4,4,4,2,3,4,8],
  stringCache:{
  },
  stringiCache:{
  },
  unpackAlignment:4,
  recordError:function recordError(errorCode) {
        if (!GL.lastError) {
          GL.lastError = errorCode;
        }
      },
  getNewId:function(table) {
        var ret = GL.counter++;
        for (var i = table.length; i < ret; i++) {
          table[i] = null;
        }
        return ret;
      },
  MAX_TEMP_BUFFER_SIZE:2097152,
  numTempVertexBuffersPerSize:64,
  log2ceilLookup:function(i) {
        return 32 - Math.clz32(i === 0 ? 0 : i - 1);
      },
  generateTempBuffers:function(quads, context) {
        var largestIndex = GL.log2ceilLookup(GL.MAX_TEMP_BUFFER_SIZE);
        context.tempVertexBufferCounters1 = [];
        context.tempVertexBufferCounters2 = [];
        context.tempVertexBufferCounters1.length = context.tempVertexBufferCounters2.length = largestIndex+1;
        context.tempVertexBuffers1 = [];
        context.tempVertexBuffers2 = [];
        context.tempVertexBuffers1.length = context.tempVertexBuffers2.length = largestIndex+1;
        context.tempIndexBuffers = [];
        context.tempIndexBuffers.length = largestIndex+1;
        for (var i = 0; i <= largestIndex; ++i) {
          context.tempIndexBuffers[i] = null; // Created on-demand
          context.tempVertexBufferCounters1[i] = context.tempVertexBufferCounters2[i] = 0;
          var ringbufferLength = GL.numTempVertexBuffersPerSize;
          context.tempVertexBuffers1[i] = [];
          context.tempVertexBuffers2[i] = [];
          var ringbuffer1 = context.tempVertexBuffers1[i];
          var ringbuffer2 = context.tempVertexBuffers2[i];
          ringbuffer1.length = ringbuffer2.length = ringbufferLength;
          for (var j = 0; j < ringbufferLength; ++j) {
            ringbuffer1[j] = ringbuffer2[j] = null; // Created on-demand
          }
        }
  
        if (quads) {
          // GL_QUAD indexes can be precalculated
          context.tempQuadIndexBuffer = GLctx.createBuffer();
          context.GLctx.bindBuffer(0x8893 /*GL_ELEMENT_ARRAY_BUFFER*/, context.tempQuadIndexBuffer);
          var numIndexes = GL.MAX_TEMP_BUFFER_SIZE >> 1;
          var quadIndexes = new Uint16Array(numIndexes);
          var i = 0, v = 0;
          while (1) {
            quadIndexes[i++] = v;
            if (i >= numIndexes) break;
            quadIndexes[i++] = v+1;
            if (i >= numIndexes) break;
            quadIndexes[i++] = v+2;
            if (i >= numIndexes) break;
            quadIndexes[i++] = v;
            if (i >= numIndexes) break;
            quadIndexes[i++] = v+2;
            if (i >= numIndexes) break;
            quadIndexes[i++] = v+3;
            if (i >= numIndexes) break;
            v += 4;
          }
          context.GLctx.bufferData(0x8893 /*GL_ELEMENT_ARRAY_BUFFER*/, quadIndexes, 0x88E4 /*GL_STATIC_DRAW*/);
          context.GLctx.bindBuffer(0x8893 /*GL_ELEMENT_ARRAY_BUFFER*/, null);
        }
      },
  getTempVertexBuffer:function getTempVertexBuffer(sizeBytes) {
        var idx = GL.log2ceilLookup(sizeBytes);
        var ringbuffer = GL.currentContext.tempVertexBuffers1[idx];
        var nextFreeBufferIndex = GL.currentContext.tempVertexBufferCounters1[idx];
        GL.currentContext.tempVertexBufferCounters1[idx] = (GL.currentContext.tempVertexBufferCounters1[idx]+1) & (GL.numTempVertexBuffersPerSize-1);
        var vbo = ringbuffer[nextFreeBufferIndex];
        if (vbo) {
          return vbo;
        }
        var prevVBO = GLctx.getParameter(0x8894 /*GL_ARRAY_BUFFER_BINDING*/);
        ringbuffer[nextFreeBufferIndex] = GLctx.createBuffer();
        GLctx.bindBuffer(0x8892 /*GL_ARRAY_BUFFER*/, ringbuffer[nextFreeBufferIndex]);
        GLctx.bufferData(0x8892 /*GL_ARRAY_BUFFER*/, 1 << idx, 0x88E8 /*GL_DYNAMIC_DRAW*/);
        GLctx.bindBuffer(0x8892 /*GL_ARRAY_BUFFER*/, prevVBO);
        return ringbuffer[nextFreeBufferIndex];
      },
  getTempIndexBuffer:function getTempIndexBuffer(sizeBytes) {
        var idx = GL.log2ceilLookup(sizeBytes);
        var ibo = GL.currentContext.tempIndexBuffers[idx];
        if (ibo) {
          return ibo;
        }
        var prevIBO = GLctx.getParameter(0x8895 /*ELEMENT_ARRAY_BUFFER_BINDING*/);
        GL.currentContext.tempIndexBuffers[idx] = GLctx.createBuffer();
        GLctx.bindBuffer(0x8893 /*GL_ELEMENT_ARRAY_BUFFER*/, GL.currentContext.tempIndexBuffers[idx]);
        GLctx.bufferData(0x8893 /*GL_ELEMENT_ARRAY_BUFFER*/, 1 << idx, 0x88E8 /*GL_DYNAMIC_DRAW*/);
        GLctx.bindBuffer(0x8893 /*GL_ELEMENT_ARRAY_BUFFER*/, prevIBO);
        return GL.currentContext.tempIndexBuffers[idx];
      },
  newRenderingFrameStarted:function newRenderingFrameStarted() {
        if (!GL.currentContext) {
          return;
        }
        var vb = GL.currentContext.tempVertexBuffers1;
        GL.currentContext.tempVertexBuffers1 = GL.currentContext.tempVertexBuffers2;
        GL.currentContext.tempVertexBuffers2 = vb;
        vb = GL.currentContext.tempVertexBufferCounters1;
        GL.currentContext.tempVertexBufferCounters1 = GL.currentContext.tempVertexBufferCounters2;
        GL.currentContext.tempVertexBufferCounters2 = vb;
        var largestIndex = GL.log2ceilLookup(GL.MAX_TEMP_BUFFER_SIZE);
        for (var i = 0; i <= largestIndex; ++i) {
          GL.currentContext.tempVertexBufferCounters1[i] = 0;
        }
      },
  getSource:function(shader, count, string, length) {
        var source = '';
        for (var i = 0; i < count; ++i) {
          var len = length ? HEAP32[(((length)+(i*4))>>2)] : -1;
          source += UTF8ToString(HEAP32[(((string)+(i*4))>>2)], len < 0 ? undefined : len);
        }
        return source;
      },
  calcBufLength:function calcBufLength(size, type, stride, count) {
        if (stride > 0) {
          return count * stride;  // XXXvlad this is not exactly correct I don't think
        }
        var typeSize = GL.byteSizeByType[type - GL.byteSizeByTypeRoot];
        return size * typeSize * count;
      },
  usedTempBuffers:[],
  preDrawHandleClientVertexAttribBindings:function preDrawHandleClientVertexAttribBindings(count) {
        GL.resetBufferBinding = false;
  
        // TODO: initial pass to detect ranges we need to upload, might not need an upload per attrib
        for (var i = 0; i < GL.currentContext.maxVertexAttribs; ++i) {
          var cb = GL.currentContext.clientBuffers[i];
          if (!cb.clientside || !cb.enabled) continue;
  
          GL.resetBufferBinding = true;
  
          var size = GL.calcBufLength(cb.size, cb.type, cb.stride, count);
          var buf = GL.getTempVertexBuffer(size);
          GLctx.bindBuffer(0x8892 /*GL_ARRAY_BUFFER*/, buf);
          GLctx.bufferSubData(0x8892 /*GL_ARRAY_BUFFER*/,
                                   0,
                                   HEAPU8.subarray(cb.ptr, cb.ptr + size));
          cb.vertexAttribPointerAdaptor.call(GLctx, i, cb.size, cb.type, cb.normalized, cb.stride, 0);
        }
      },
  postDrawHandleClientVertexAttribBindings:function postDrawHandleClientVertexAttribBindings() {
        if (GL.resetBufferBinding) {
          GLctx.bindBuffer(0x8892 /*GL_ARRAY_BUFFER*/, GL.buffers[GLctx.currentArrayBufferBinding]);
        }
      },
  createContext:function(/** @type {HTMLCanvasElement} */ canvas, webGLContextAttributes) {
  
        // BUG: Workaround Safari WebGL issue: After successfully acquiring WebGL context on a canvas,
        // calling .getContext() will always return that context independent of which 'webgl' or 'webgl2'
        // context version was passed. See https://bugs.webkit.org/show_bug.cgi?id=222758 and
        // https://github.com/emscripten-core/emscripten/issues/13295.
        // TODO: Once the bug is fixed and shipped in Safari, adjust the Safari version field in above check.
        if (!canvas.getContextSafariWebGL2Fixed) {
          canvas.getContextSafariWebGL2Fixed = canvas.getContext;
          /** @type {function(this:HTMLCanvasElement, string, (Object|null)=): (Object|null)} */
          function fixedGetContext(ver, attrs) {
            var gl = canvas.getContextSafariWebGL2Fixed(ver, attrs);
            return ((ver == 'webgl') == (gl instanceof WebGLRenderingContext)) ? gl : null;
          }
          canvas.getContext = fixedGetContext;
        }
  
        var ctx =
          (webGLContextAttributes.majorVersion > 1)
          ?
            canvas.getContext("webgl2", webGLContextAttributes)
          :
          (canvas.getContext("webgl", webGLContextAttributes)
            // https://caniuse.com/#feat=webgl
            );
  
        if (!ctx) return 0;
  
        var handle = GL.registerContext(ctx, webGLContextAttributes);
  
        return handle;
      },
  registerContext:function(ctx, webGLContextAttributes) {
        // without pthreads a context is just an integer ID
        var handle = GL.getNewId(GL.contexts);
  
        var context = {
          handle,
          attributes: webGLContextAttributes,
          version: webGLContextAttributes.majorVersion,
          GLctx: ctx
        };
  
        // Store the created context object so that we can access the context given a canvas without having to pass the parameters again.
        if (ctx.canvas) ctx.canvas.GLctxObject = context;
        GL.contexts[handle] = context;
        if (typeof webGLContextAttributes.enableExtensionsByDefault == 'undefined' || webGLContextAttributes.enableExtensionsByDefault) {
          GL.initExtensions(context);
        }
  
        context.maxVertexAttribs = context.GLctx.getParameter(0x8869 /*GL_MAX_VERTEX_ATTRIBS*/);
        context.clientBuffers = [];
        for (var i = 0; i < context.maxVertexAttribs; i++) {
          context.clientBuffers[i] = { enabled: false, clientside: false, size: 0, type: 0, normalized: 0, stride: 0, ptr: 0, vertexAttribPointerAdaptor: null };
        }
  
        GL.generateTempBuffers(false, context);
  
        return handle;
      },
  makeContextCurrent:function(contextHandle) {
  
        GL.currentContext = GL.contexts[contextHandle]; // Active Emscripten GL layer context object.
        Module.ctx = GLctx = GL.currentContext && GL.currentContext.GLctx; // Active WebGL context object.
        return !(contextHandle && !GLctx);
      },
  getContext:function(contextHandle) {
        return GL.contexts[contextHandle];
      },
  deleteContext:function(contextHandle) {
        if (GL.currentContext === GL.contexts[contextHandle]) GL.currentContext = null;
        if (typeof JSEvents == 'object') JSEvents.removeAllHandlersOnTarget(GL.contexts[contextHandle].GLctx.canvas); // Release all JS event handlers on the DOM element that the GL context is associated with since the context is now deleted.
        if (GL.contexts[contextHandle] && GL.contexts[contextHandle].GLctx.canvas) GL.contexts[contextHandle].GLctx.canvas.GLctxObject = undefined; // Make sure the canvas object no longer refers to the context object so there are no GC surprises.
        GL.contexts[contextHandle] = null;
      },
  initExtensions:function(context) {
        // If this function is called without a specific context object, init the extensions of the currently active context.
        if (!context) context = GL.currentContext;
  
        if (context.initExtensionsDone) return;
        context.initExtensionsDone = true;
  
        var GLctx = context.GLctx;
  
        // Detect the presence of a few extensions manually, this GL interop layer itself will need to know if they exist.
  
        // Extensions that are only available in WebGL 1 (the calls will be no-ops if called on a WebGL 2 context active)
        webgl_enable_ANGLE_instanced_arrays(GLctx);
        webgl_enable_OES_vertex_array_object(GLctx);
        webgl_enable_WEBGL_draw_buffers(GLctx);
        // Extensions that are available from WebGL >= 2 (no-op if called on a WebGL 1 context active)
        webgl_enable_WEBGL_draw_instanced_base_vertex_base_instance(GLctx);
        webgl_enable_WEBGL_multi_draw_instanced_base_vertex_base_instance(GLctx);
  
        // On WebGL 2, EXT_disjoint_timer_query is replaced with an alternative
        // that's based on core APIs, and exposes only the queryCounterEXT()
        // entrypoint.
        if (context.version >= 2) {
          GLctx.disjointTimerQueryExt = GLctx.getExtension("EXT_disjoint_timer_query_webgl2");
        }
  
        // However, Firefox exposes the WebGL 1 version on WebGL 2 as well and
        // thus we look for the WebGL 1 version again if the WebGL 2 version
        // isn't present. https://bugzilla.mozilla.org/show_bug.cgi?id=1328882
        if (context.version < 2 || !GLctx.disjointTimerQueryExt)
        {
          GLctx.disjointTimerQueryExt = GLctx.getExtension("EXT_disjoint_timer_query");
        }
  
        webgl_enable_WEBGL_multi_draw(GLctx);
  
        // .getSupportedExtensions() can return null if context is lost, so coerce to empty array.
        var exts = GLctx.getSupportedExtensions() || [];
        exts.forEach(function(ext) {
          // WEBGL_lose_context, WEBGL_debug_renderer_info and WEBGL_debug_shaders are not enabled by default.
          if (!ext.includes('lose_context') && !ext.includes('debug')) {
            // Call .getExtension() to enable that extension permanently.
            GLctx.getExtension(ext);
          }
        });
      },
  };
  
  function _eglCreateContext(display, config, hmm, contextAttribs) {
      if (display != 62000 /* Magic ID for Emscripten 'default display' */) {
        EGL.setErrorCode(0x3008 /* EGL_BAD_DISPLAY */);
        return 0;
      }
  
      // EGL 1.4 spec says default EGL_CONTEXT_CLIENT_VERSION is GLES1, but this is not supported by Emscripten.
      // So user must pass EGL_CONTEXT_CLIENT_VERSION == 2 to initialize EGL.
      var glesContextVersion = 1;
      for (;;) {
        var param = HEAP32[((contextAttribs)>>2)];
        if (param == 0x3098 /*EGL_CONTEXT_CLIENT_VERSION*/) {
          glesContextVersion = HEAP32[(((contextAttribs)+(4))>>2)];
        } else if (param == 0x3038 /*EGL_NONE*/) {
          break;
        } else {
          /* EGL1.4 specifies only EGL_CONTEXT_CLIENT_VERSION as supported attribute */
          EGL.setErrorCode(0x3004 /*EGL_BAD_ATTRIBUTE*/);
          return 0;
        }
        contextAttribs += 8;
      }
      if (glesContextVersion < 2 || glesContextVersion > 3) {
        EGL.setErrorCode(0x3005 /* EGL_BAD_CONFIG */);
        return 0; /* EGL_NO_CONTEXT */
      }
  
      EGL.contextAttributes.majorVersion = glesContextVersion - 1; // WebGL 1 is GLES 2, WebGL2 is GLES3
      EGL.contextAttributes.minorVersion = 0;
  
      EGL.context = GL.createContext(Module['canvas'], EGL.contextAttributes);
  
      if (EGL.context != 0) {
        EGL.setErrorCode(0x3000 /* EGL_SUCCESS */);
  
        // Run callbacks so that GL emulation works
        GL.makeContextCurrent(EGL.context);
        Module.useWebGL = true;
        Browser.moduleContextCreatedCallbacks.forEach(function(callback) { callback() });
  
        // Note: This function only creates a context, but it shall not make it active.
        GL.makeContextCurrent(null);
        return 62004; // Magic ID for Emscripten EGLContext
      } else {
        EGL.setErrorCode(0x3009 /* EGL_BAD_MATCH */); // By the EGL 1.4 spec, an implementation that does not support GLES2 (WebGL in this case), this error code is set.
        return 0; /* EGL_NO_CONTEXT */
      }
    }

  function _eglCreateWindowSurface(display, config, win, attrib_list) {
      if (display != 62000 /* Magic ID for Emscripten 'default display' */) {
        EGL.setErrorCode(0x3008 /* EGL_BAD_DISPLAY */);
        return 0;
      }
      if (config != 62002 /* Magic ID for the only EGLConfig supported by Emscripten */) {
        EGL.setErrorCode(0x3005 /* EGL_BAD_CONFIG */);
        return 0;
      }
      // TODO: Examine attrib_list! Parameters that can be present there are:
      // - EGL_RENDER_BUFFER (must be EGL_BACK_BUFFER)
      // - EGL_VG_COLORSPACE (can't be set)
      // - EGL_VG_ALPHA_FORMAT (can't be set)
      EGL.setErrorCode(0x3000 /* EGL_SUCCESS */);
      return 62006; /* Magic ID for Emscripten 'default surface' */
    }

  
  function _eglDestroyContext(display, context) {
      if (display != 62000 /* Magic ID for Emscripten 'default display' */) {
        EGL.setErrorCode(0x3008 /* EGL_BAD_DISPLAY */);
        return 0;
      }
      if (context != 62004 /* Magic ID for Emscripten EGLContext */) {
        EGL.setErrorCode(0x3006 /* EGL_BAD_CONTEXT */);
        return 0;
      }
  
      GL.deleteContext(EGL.context);
      EGL.setErrorCode(0x3000 /* EGL_SUCCESS */);
      if (EGL.currentContext == context) {
        EGL.currentContext = 0;
      }
      return 1 /* EGL_TRUE */;
    }

  function _eglDestroySurface(display, surface) {
      if (display != 62000 /* Magic ID for Emscripten 'default display' */) {
        EGL.setErrorCode(0x3008 /* EGL_BAD_DISPLAY */);
        return 0;
      }
      if (surface != 62006 /* Magic ID for the only EGLSurface supported by Emscripten */) {
        EGL.setErrorCode(0x300D /* EGL_BAD_SURFACE */);
        return 1;
      }
      if (EGL.currentReadSurface == surface) {
        EGL.currentReadSurface = 0;
      }
      if (EGL.currentDrawSurface == surface) {
        EGL.currentDrawSurface = 0;
      }
      EGL.setErrorCode(0x3000 /* EGL_SUCCESS */);
      return 1; /* Magic ID for Emscripten 'default surface' */
    }

  function _eglGetConfigAttrib(display, config, attribute, value) {
      if (display != 62000 /* Magic ID for Emscripten 'default display' */) {
        EGL.setErrorCode(0x3008 /* EGL_BAD_DISPLAY */);
        return 0;
      }
      if (config != 62002 /* Magic ID for the only EGLConfig supported by Emscripten */) {
        EGL.setErrorCode(0x3005 /* EGL_BAD_CONFIG */);
        return 0;
      }
      if (!value) {
        EGL.setErrorCode(0x300C /* EGL_BAD_PARAMETER */);
        return 0;
      }
      EGL.setErrorCode(0x3000 /* EGL_SUCCESS */);
      switch (attribute) {
      case 0x3020: // EGL_BUFFER_SIZE
        HEAP32[((value)>>2)] = EGL.contextAttributes.alpha ? 32 : 24;
        return 1;
      case 0x3021: // EGL_ALPHA_SIZE
        HEAP32[((value)>>2)] = EGL.contextAttributes.alpha ? 8 : 0;
        return 1;
      case 0x3022: // EGL_BLUE_SIZE
        HEAP32[((value)>>2)] = 8;
        return 1;
      case 0x3023: // EGL_GREEN_SIZE
        HEAP32[((value)>>2)] = 8;
        return 1;
      case 0x3024: // EGL_RED_SIZE
        HEAP32[((value)>>2)] = 8;
        return 1;
      case 0x3025: // EGL_DEPTH_SIZE
        HEAP32[((value)>>2)] = EGL.contextAttributes.depth ? 24 : 0;
        return 1;
      case 0x3026: // EGL_STENCIL_SIZE
        HEAP32[((value)>>2)] = EGL.contextAttributes.stencil ? 8 : 0;
        return 1;
      case 0x3027: // EGL_CONFIG_CAVEAT
        // We can return here one of EGL_NONE (0x3038), EGL_SLOW_CONFIG (0x3050) or EGL_NON_CONFORMANT_CONFIG (0x3051).
        HEAP32[((value)>>2)] = 0x3038;
        return 1;
      case 0x3028: // EGL_CONFIG_ID
        HEAP32[((value)>>2)] = 62002;
        return 1;
      case 0x3029: // EGL_LEVEL
        HEAP32[((value)>>2)] = 0;
        return 1;
      case 0x302A: // EGL_MAX_PBUFFER_HEIGHT
        HEAP32[((value)>>2)] = 4096;
        return 1;
      case 0x302B: // EGL_MAX_PBUFFER_PIXELS
        HEAP32[((value)>>2)] = 16777216;
        return 1;
      case 0x302C: // EGL_MAX_PBUFFER_WIDTH
        HEAP32[((value)>>2)] = 4096;
        return 1;
      case 0x302D: // EGL_NATIVE_RENDERABLE
        HEAP32[((value)>>2)] = 0;
        return 1;
      case 0x302E: // EGL_NATIVE_VISUAL_ID
        HEAP32[((value)>>2)] = 0;
        return 1;
      case 0x302F: // EGL_NATIVE_VISUAL_TYPE
        HEAP32[((value)>>2)] = 0x3038;
        return 1;
      case 0x3031: // EGL_SAMPLES
        HEAP32[((value)>>2)] = EGL.contextAttributes.antialias ? 4 : 0;
        return 1;
      case 0x3032: // EGL_SAMPLE_BUFFERS
        HEAP32[((value)>>2)] = EGL.contextAttributes.antialias ? 1 : 0;
        return 1;
      case 0x3033: // EGL_SURFACE_TYPE
        HEAP32[((value)>>2)] = 0x4;
        return 1;
      case 0x3034: // EGL_TRANSPARENT_TYPE
        // If this returns EGL_TRANSPARENT_RGB (0x3052), transparency is used through color-keying. No such thing applies to Emscripten canvas.
        HEAP32[((value)>>2)] = 0x3038;
        return 1;
      case 0x3035: // EGL_TRANSPARENT_BLUE_VALUE
      case 0x3036: // EGL_TRANSPARENT_GREEN_VALUE
      case 0x3037: // EGL_TRANSPARENT_RED_VALUE
        // "If EGL_TRANSPARENT_TYPE is EGL_NONE, then the values for EGL_TRANSPARENT_RED_VALUE, EGL_TRANSPARENT_GREEN_VALUE, and EGL_TRANSPARENT_BLUE_VALUE are undefined."
        HEAP32[((value)>>2)] = -1;
        return 1;
      case 0x3039: // EGL_BIND_TO_TEXTURE_RGB
      case 0x303A: // EGL_BIND_TO_TEXTURE_RGBA
        HEAP32[((value)>>2)] = 0;
        return 1;
      case 0x303B: // EGL_MIN_SWAP_INTERVAL
        HEAP32[((value)>>2)] = 0;
        return 1;
      case 0x303C: // EGL_MAX_SWAP_INTERVAL
        HEAP32[((value)>>2)] = 1;
        return 1;
      case 0x303D: // EGL_LUMINANCE_SIZE
      case 0x303E: // EGL_ALPHA_MASK_SIZE
        HEAP32[((value)>>2)] = 0;
        return 1;
      case 0x303F: // EGL_COLOR_BUFFER_TYPE
        // EGL has two types of buffers: EGL_RGB_BUFFER and EGL_LUMINANCE_BUFFER.
        HEAP32[((value)>>2)] = 0x308E;
        return 1;
      case 0x3040: // EGL_RENDERABLE_TYPE
        // A bit combination of EGL_OPENGL_ES_BIT,EGL_OPENVG_BIT,EGL_OPENGL_ES2_BIT and EGL_OPENGL_BIT.
        HEAP32[((value)>>2)] = 0x4;
        return 1;
      case 0x3042: // EGL_CONFORMANT
        // "EGL_CONFORMANT is a mask indicating if a client API context created with respect to the corresponding EGLConfig will pass the required conformance tests for that API."
        HEAP32[((value)>>2)] = 0;
        return 1;
      default:
        EGL.setErrorCode(0x3004 /* EGL_BAD_ATTRIBUTE */);
        return 0;
      }
    }

  function _eglGetDisplay(nativeDisplayType) {
      EGL.setErrorCode(0x3000 /* EGL_SUCCESS */);
      // Note: As a 'conformant' implementation of EGL, we would prefer to init here only if the user
      //       calls this function with EGL_DEFAULT_DISPLAY. Other display IDs would be preferred to be unsupported
      //       and EGL_NO_DISPLAY returned. Uncomment the following code lines to do this.
      // Instead, an alternative route has been preferred, namely that the Emscripten EGL implementation
      // "emulates" X11, and eglGetDisplay is expected to accept/receive a pointer to an X11 Display object.
      // Therefore, be lax and allow anything to be passed in, and return the magic handle to our default EGLDisplay object.
  
  //    if (nativeDisplayType == 0 /* EGL_DEFAULT_DISPLAY */) {
          return 62000; // Magic ID for Emscripten 'default display'
  //    }
  //    else
  //      return 0; // EGL_NO_DISPLAY
    }

  function _eglGetError() {
      return EGL.errorCode;
    }

  function _eglInitialize(display, majorVersion, minorVersion) {
      if (display != 62000 /* Magic ID for Emscripten 'default display' */) {
        EGL.setErrorCode(0x3008 /* EGL_BAD_DISPLAY */);
        return 0;
      }
      if (majorVersion) {
        HEAP32[((majorVersion)>>2)] = 1; // Advertise EGL Major version: '1'
      }
      if (minorVersion) {
        HEAP32[((minorVersion)>>2)] = 4; // Advertise EGL Minor version: '4'
      }
      EGL.defaultDisplayInitialized = true;
      EGL.setErrorCode(0x3000 /* EGL_SUCCESS */);
      return 1;
    }

  
  function _eglMakeCurrent(display, draw, read, context) {
      if (display != 62000 /* Magic ID for Emscripten 'default display' */) {
        EGL.setErrorCode(0x3008 /* EGL_BAD_DISPLAY */);
        return 0 /* EGL_FALSE */;
      }
      //\todo An EGL_NOT_INITIALIZED error is generated if EGL is not initialized for dpy.
      if (context != 0 && context != 62004 /* Magic ID for Emscripten EGLContext */) {
        EGL.setErrorCode(0x3006 /* EGL_BAD_CONTEXT */);
        return 0;
      }
      if ((read != 0 && read != 62006) || (draw != 0 && draw != 62006 /* Magic ID for Emscripten 'default surface' */)) {
        EGL.setErrorCode(0x300D /* EGL_BAD_SURFACE */);
        return 0;
      }
  
      GL.makeContextCurrent(context ? EGL.context : null);
  
      EGL.currentContext = context;
      EGL.currentDrawSurface = draw;
      EGL.currentReadSurface = read;
      EGL.setErrorCode(0x3000 /* EGL_SUCCESS */);
      return 1 /* EGL_TRUE */;
    }

  
  
  var stringToNewUTF8 = (str) => {
      var size = lengthBytesUTF8(str) + 1;
      var ret = _malloc(size);
      if (ret) stringToUTF8(str, ret, size);
      return ret;
    };
  
  function _eglQueryString(display, name) {
      if (display != 62000 /* Magic ID for Emscripten 'default display' */) {
        EGL.setErrorCode(0x3008 /* EGL_BAD_DISPLAY */);
        return 0;
      }
      //\todo An EGL_NOT_INITIALIZED error is generated if EGL is not initialized for dpy.
      EGL.setErrorCode(0x3000 /* EGL_SUCCESS */);
      if (EGL.stringCache[name]) return EGL.stringCache[name];
      var ret;
      switch (name) {
        case 0x3053 /* EGL_VENDOR */: ret = stringToNewUTF8("Emscripten"); break;
        case 0x3054 /* EGL_VERSION */: ret = stringToNewUTF8("1.4 Emscripten EGL"); break;
        case 0x3055 /* EGL_EXTENSIONS */:  ret = stringToNewUTF8(""); break; // Currently not supporting any EGL extensions.
        case 0x308D /* EGL_CLIENT_APIS */: ret = stringToNewUTF8("OpenGL_ES"); break;
        default:
          EGL.setErrorCode(0x300C /* EGL_BAD_PARAMETER */);
          return 0;
      }
      EGL.stringCache[name] = ret;
      return ret;
    }

  function _eglSwapBuffers(dpy, surface) {
  
      if (!EGL.defaultDisplayInitialized) {
        EGL.setErrorCode(0x3001 /* EGL_NOT_INITIALIZED */);
      } else if (!Module.ctx) {
        EGL.setErrorCode(0x3002 /* EGL_BAD_ACCESS */);
      } else if (Module.ctx.isContextLost()) {
        EGL.setErrorCode(0x300E /* EGL_CONTEXT_LOST */);
      } else {
        // According to documentation this does an implicit flush.
        // Due to discussion at https://github.com/emscripten-core/emscripten/pull/1871
        // the flush was removed since this _may_ result in slowing code down.
        //_glFlush();
        EGL.setErrorCode(0x3000 /* EGL_SUCCESS */);
        return 1 /* EGL_TRUE */;
      }
      return 0 /* EGL_FALSE */;
    }

  
  function _eglSwapInterval(display, interval) {
      if (display != 62000 /* Magic ID for Emscripten 'default display' */) {
        EGL.setErrorCode(0x3008 /* EGL_BAD_DISPLAY */);
        return 0;
      }
      if (interval == 0) _emscripten_set_main_loop_timing(0, 0);
      else _emscripten_set_main_loop_timing(1, interval);
  
      EGL.setErrorCode(0x3000 /* EGL_SUCCESS */);
      return 1;
    }

  function _eglTerminate(display) {
      if (display != 62000 /* Magic ID for Emscripten 'default display' */) {
        EGL.setErrorCode(0x3008 /* EGL_BAD_DISPLAY */);
        return 0;
      }
      EGL.currentContext = 0;
      EGL.currentReadSurface = 0;
      EGL.currentDrawSurface = 0;
      EGL.defaultDisplayInitialized = false;
      EGL.setErrorCode(0x3000 /* EGL_SUCCESS */);
      return 1;
    }

  
  /** @suppress {duplicate } */
  function _eglWaitClient() {
      EGL.setErrorCode(0x3000 /* EGL_SUCCESS */);
      return 1;
    }
  var _eglWaitGL = _eglWaitClient;

  function _eglWaitNative(nativeEngineId) {
      EGL.setErrorCode(0x3000 /* EGL_SUCCESS */);
      return 1;
    }

  var readEmAsmArgsArray = [];
  var readEmAsmArgs = (sigPtr, buf) => {
      // Nobody should have mutated _readEmAsmArgsArray underneath us to be something else than an array.
      assert(Array.isArray(readEmAsmArgsArray));
      // The input buffer is allocated on the stack, so it must be stack-aligned.
      assert(buf % 16 == 0);
      readEmAsmArgsArray.length = 0;
      var ch;
      // Most arguments are i32s, so shift the buffer pointer so it is a plain
      // index into HEAP32.
      buf >>= 2;
      while (ch = HEAPU8[sigPtr++]) {
        var chr = String.fromCharCode(ch);
        var validChars = ['d', 'f', 'i'];
        assert(validChars.includes(chr), `Invalid character ${ch}("${chr}") in readEmAsmArgs! Use only [${validChars}], and do not specify "v" for void return argument.`);
        // Floats are always passed as doubles, and doubles and int64s take up 8
        // bytes (two 32-bit slots) in memory, align reads to these:
        buf += (ch != 105/*i*/) & buf;
        readEmAsmArgsArray.push(
          ch == 105/*i*/ ? HEAP32[buf] :
         HEAPF64[buf++ >> 1]
        );
        ++buf;
      }
      return readEmAsmArgsArray;
    };
  var runEmAsmFunction = (code, sigPtr, argbuf) => {
      var args = readEmAsmArgs(sigPtr, argbuf);
      if (!ASM_CONSTS.hasOwnProperty(code)) abort(`No EM_ASM constant found at address ${code}`);
      return ASM_CONSTS[code].apply(null, args);
    };
  var _emscripten_asm_const_int = (code, sigPtr, argbuf) => {
      return runEmAsmFunction(code, sigPtr, argbuf);
    };

  var runMainThreadEmAsm = (code, sigPtr, argbuf, sync) => {
      var args = readEmAsmArgs(sigPtr, argbuf);
      if (!ASM_CONSTS.hasOwnProperty(code)) abort(`No EM_ASM constant found at address ${code}`);
      return ASM_CONSTS[code].apply(null, args);
    };
  var _emscripten_asm_const_int_sync_on_main_thread = (code, sigPtr, argbuf) => {
      return runMainThreadEmAsm(code, sigPtr, argbuf, 1);
    };

  function _emscripten_cancel_main_loop() {
      Browser.mainLoop.pause();
      Browser.mainLoop.func = null;
    }

  var _emscripten_console_error = (str) => {
      assert(typeof str == 'number');
      console.error(UTF8ToString(str));
    };

  function _emscripten_date_now() {
      return Date.now();
    }

  var withStackSave = (f) => {
      var stack = stackSave();
      var ret = f();
      stackRestore(stack);
      return ret;
    };
  var JSEvents = {
  inEventHandler:0,
  removeAllEventListeners:function() {
        for (var i = JSEvents.eventHandlers.length-1; i >= 0; --i) {
          JSEvents._removeHandler(i);
        }
        JSEvents.eventHandlers = [];
        JSEvents.deferredCalls = [];
      },
  registerRemoveEventListeners:function() {
        if (!JSEvents.removeEventListenersRegistered) {
          __ATEXIT__.push(JSEvents.removeAllEventListeners);
          JSEvents.removeEventListenersRegistered = true;
        }
      },
  deferredCalls:[],
  deferCall:function(targetFunction, precedence, argsList) {
        function arraysHaveEqualContent(arrA, arrB) {
          if (arrA.length != arrB.length) return false;
  
          for (var i in arrA) {
            if (arrA[i] != arrB[i]) return false;
          }
          return true;
        }
        // Test if the given call was already queued, and if so, don't add it again.
        for (var i in JSEvents.deferredCalls) {
          var call = JSEvents.deferredCalls[i];
          if (call.targetFunction == targetFunction && arraysHaveEqualContent(call.argsList, argsList)) {
            return;
          }
        }
        JSEvents.deferredCalls.push({
          targetFunction,
          precedence,
          argsList
        });
  
        JSEvents.deferredCalls.sort(function(x,y) { return x.precedence < y.precedence; });
      },
  removeDeferredCalls:function(targetFunction) {
        for (var i = 0; i < JSEvents.deferredCalls.length; ++i) {
          if (JSEvents.deferredCalls[i].targetFunction == targetFunction) {
            JSEvents.deferredCalls.splice(i, 1);
            --i;
          }
        }
      },
  canPerformEventHandlerRequests:function() {
        if (navigator.userActivation) {
          // Verify against transient activation status from UserActivation API
          // whether it is possible to perform a request here without needing to defer. See
          // https://developer.mozilla.org/en-US/docs/Web/Security/User_activation#transient_activation
          // and https://caniuse.com/mdn-api_useractivation
          // At the time of writing, Firefox does not support this API: https://bugzilla.mozilla.org/show_bug.cgi?id=1791079
          return navigator.userActivation.isActive;
        }
        return JSEvents.inEventHandler && JSEvents.currentEventHandler.allowsDeferredCalls;
      },
  runDeferredCalls:function() {
        if (!JSEvents.canPerformEventHandlerRequests()) {
          return;
        }
        for (var i = 0; i < JSEvents.deferredCalls.length; ++i) {
          var call = JSEvents.deferredCalls[i];
          JSEvents.deferredCalls.splice(i, 1);
          --i;
          call.targetFunction.apply(null, call.argsList);
        }
      },
  eventHandlers:[],
  removeAllHandlersOnTarget:function(target, eventTypeString) {
        for (var i = 0; i < JSEvents.eventHandlers.length; ++i) {
          if (JSEvents.eventHandlers[i].target == target && 
            (!eventTypeString || eventTypeString == JSEvents.eventHandlers[i].eventTypeString)) {
             JSEvents._removeHandler(i--);
           }
        }
      },
  _removeHandler:function(i) {
        var h = JSEvents.eventHandlers[i];
        h.target.removeEventListener(h.eventTypeString, h.eventListenerFunc, h.useCapture);
        JSEvents.eventHandlers.splice(i, 1);
      },
  registerOrRemoveHandler:function(eventHandler) {
        if (!eventHandler.target) {
          err('registerOrRemoveHandler: the target element for event handler registration does not exist, when processing the following event handler registration:');
          console.dir(eventHandler);
          return -4;
        }
        var jsEventHandler = function jsEventHandler(event) {
          // Increment nesting count for the event handler.
          ++JSEvents.inEventHandler;
          JSEvents.currentEventHandler = eventHandler;
          // Process any old deferred calls the user has placed.
          JSEvents.runDeferredCalls();
          // Process the actual event, calls back to user C code handler.
          eventHandler.handlerFunc(event);
          // Process any new deferred calls that were placed right now from this event handler.
          JSEvents.runDeferredCalls();
          // Out of event handler - restore nesting count.
          --JSEvents.inEventHandler;
        };
        
        if (eventHandler.callbackfunc) {
          eventHandler.eventListenerFunc = jsEventHandler;
          eventHandler.target.addEventListener(eventHandler.eventTypeString, jsEventHandler, eventHandler.useCapture);
          JSEvents.eventHandlers.push(eventHandler);
          JSEvents.registerRemoveEventListeners();
        } else {
          for (var i = 0; i < JSEvents.eventHandlers.length; ++i) {
            if (JSEvents.eventHandlers[i].target == eventHandler.target
             && JSEvents.eventHandlers[i].eventTypeString == eventHandler.eventTypeString) {
               JSEvents._removeHandler(i--);
             }
          }
        }
        return 0;
      },
  getNodeNameForTarget:function(target) {
        if (!target) return '';
        if (target == window) return '#window';
        if (target == screen) return '#screen';
        return (target && target.nodeName) ? target.nodeName : '';
      },
  fullscreenEnabled:function() {
        return document.fullscreenEnabled
        // Safari 13.0.3 on macOS Catalina 10.15.1 still ships with prefixed webkitFullscreenEnabled.
        // TODO: If Safari at some point ships with unprefixed version, update the version check above.
        || document.webkitFullscreenEnabled
         ;
      },
  };
  
  var currentFullscreenStrategy = {
  };
  
  
  
  
  function maybeCStringToJsString(cString) {
      // "cString > 2" checks if the input is a number, and isn't of the special
      // values we accept here, EMSCRIPTEN_EVENT_TARGET_* (which map to 0, 1, 2).
      // In other words, if cString > 2 then it's a pointer to a valid place in
      // memory, and points to a C string.
      return cString > 2 ? UTF8ToString(cString) : cString;
    }
  
  var specialHTMLTargets = [0, typeof document != 'undefined' ? document : 0, typeof window != 'undefined' ? window : 0];
  function findEventTarget(target) {
      target = maybeCStringToJsString(target);
      var domElement = specialHTMLTargets[target] || (typeof document != 'undefined' ? document.querySelector(target) : undefined);
      return domElement;
    }
  function findCanvasEventTarget(target) { return findEventTarget(target); }
  function _emscripten_get_canvas_element_size(target, width, height) {
      var canvas = findCanvasEventTarget(target);
      if (!canvas) return -4;
      HEAP32[((width)>>2)] = canvas.width;
      HEAP32[((height)>>2)] = canvas.height;
    }
  
  
  
  var stringToUTF8OnStack = (str) => {
      var size = lengthBytesUTF8(str) + 1;
      var ret = stackAlloc(size);
      stringToUTF8(str, ret, size);
      return ret;
    };
  var getCanvasElementSize = (target) => withStackSave(() => {
      var w = stackAlloc(8);
      var h = w + 4;
  
      var targetInt = stringToUTF8OnStack(target.id);
      var ret = _emscripten_get_canvas_element_size(targetInt, w, h);
      var size = [HEAP32[((w)>>2)], HEAP32[((h)>>2)]];
      return size;
    });
  
  
  function _emscripten_set_canvas_element_size(target, width, height) {
      var canvas = findCanvasEventTarget(target);
      if (!canvas) return -4;
      canvas.width = width;
      canvas.height = height;
      return 0;
    }
  
  
  function setCanvasElementSize(target, width, height) {
      if (!target.controlTransferredOffscreen) {
        target.width = width;
        target.height = height;
      } else {
        // This function is being called from high-level JavaScript code instead of asm.js/Wasm,
        // and it needs to synchronously proxy over to another thread, so marshal the string onto the heap to do the call.
        withStackSave(function() {
          var targetInt = stringToUTF8OnStack(target.id);
          _emscripten_set_canvas_element_size(targetInt, width, height);
        });
      }
    }
  
  function registerRestoreOldStyle(canvas) {
      var canvasSize = getCanvasElementSize(canvas);
      var oldWidth = canvasSize[0];
      var oldHeight = canvasSize[1];
      var oldCssWidth = canvas.style.width;
      var oldCssHeight = canvas.style.height;
      var oldBackgroundColor = canvas.style.backgroundColor; // Chrome reads color from here.
      var oldDocumentBackgroundColor = document.body.style.backgroundColor; // IE11 reads color from here.
      // Firefox always has black background color.
      var oldPaddingLeft = canvas.style.paddingLeft; // Chrome, FF, Safari
      var oldPaddingRight = canvas.style.paddingRight;
      var oldPaddingTop = canvas.style.paddingTop;
      var oldPaddingBottom = canvas.style.paddingBottom;
      var oldMarginLeft = canvas.style.marginLeft; // IE11
      var oldMarginRight = canvas.style.marginRight;
      var oldMarginTop = canvas.style.marginTop;
      var oldMarginBottom = canvas.style.marginBottom;
      var oldDocumentBodyMargin = document.body.style.margin;
      var oldDocumentOverflow = document.documentElement.style.overflow; // Chrome, Firefox
      var oldDocumentScroll = document.body.scroll; // IE
      var oldImageRendering = canvas.style.imageRendering;
  
      function restoreOldStyle() {
        var fullscreenElement = document.fullscreenElement
          || document.webkitFullscreenElement
          ;
        if (!fullscreenElement) {
          document.removeEventListener('fullscreenchange', restoreOldStyle);
  
          // Unprefixed Fullscreen API shipped in Chromium 71 (https://bugs.chromium.org/p/chromium/issues/detail?id=383813)
          // As of Safari 13.0.3 on macOS Catalina 10.15.1 still ships with prefixed webkitfullscreenchange. TODO: revisit this check once Safari ships unprefixed version.
          document.removeEventListener('webkitfullscreenchange', restoreOldStyle);
  
          setCanvasElementSize(canvas, oldWidth, oldHeight);
  
          canvas.style.width = oldCssWidth;
          canvas.style.height = oldCssHeight;
          canvas.style.backgroundColor = oldBackgroundColor; // Chrome
          // IE11 hack: assigning 'undefined' or an empty string to document.body.style.backgroundColor has no effect, so first assign back the default color
          // before setting the undefined value. Setting undefined value is also important, or otherwise we would later treat that as something that the user
          // had explicitly set so subsequent fullscreen transitions would not set background color properly.
          if (!oldDocumentBackgroundColor) document.body.style.backgroundColor = 'white';
          document.body.style.backgroundColor = oldDocumentBackgroundColor; // IE11
          canvas.style.paddingLeft = oldPaddingLeft; // Chrome, FF, Safari
          canvas.style.paddingRight = oldPaddingRight;
          canvas.style.paddingTop = oldPaddingTop;
          canvas.style.paddingBottom = oldPaddingBottom;
          canvas.style.marginLeft = oldMarginLeft; // IE11
          canvas.style.marginRight = oldMarginRight;
          canvas.style.marginTop = oldMarginTop;
          canvas.style.marginBottom = oldMarginBottom;
          document.body.style.margin = oldDocumentBodyMargin;
          document.documentElement.style.overflow = oldDocumentOverflow; // Chrome, Firefox
          document.body.scroll = oldDocumentScroll; // IE
          canvas.style.imageRendering = oldImageRendering;
          if (canvas.GLctxObject) canvas.GLctxObject.GLctx.viewport(0, 0, oldWidth, oldHeight);
  
          if (currentFullscreenStrategy.canvasResizedCallback) {
            getWasmTableEntry(currentFullscreenStrategy.canvasResizedCallback)(37, 0, currentFullscreenStrategy.canvasResizedCallbackUserData);
          }
        }
      }
      document.addEventListener('fullscreenchange', restoreOldStyle);
      // Unprefixed Fullscreen API shipped in Chromium 71 (https://bugs.chromium.org/p/chromium/issues/detail?id=383813)
      // As of Safari 13.0.3 on macOS Catalina 10.15.1 still ships with prefixed webkitfullscreenchange. TODO: revisit this check once Safari ships unprefixed version.
      document.addEventListener('webkitfullscreenchange', restoreOldStyle);
      return restoreOldStyle;
    }
  
  
  function setLetterbox(element, topBottom, leftRight) {
        // Cannot use margin to specify letterboxes in FF or Chrome, since those ignore margins in fullscreen mode.
        element.style.paddingLeft = element.style.paddingRight = leftRight + 'px';
        element.style.paddingTop = element.style.paddingBottom = topBottom + 'px';
    }
  
  
  function getBoundingClientRect(e) {
      return specialHTMLTargets.indexOf(e) < 0 ? e.getBoundingClientRect() : {'left':0,'top':0};
    }
  function JSEvents_resizeCanvasForFullscreen(target, strategy) {
      var restoreOldStyle = registerRestoreOldStyle(target);
      var cssWidth = strategy.softFullscreen ? innerWidth : screen.width;
      var cssHeight = strategy.softFullscreen ? innerHeight : screen.height;
      var rect = getBoundingClientRect(target);
      var windowedCssWidth = rect.width;
      var windowedCssHeight = rect.height;
      var canvasSize = getCanvasElementSize(target);
      var windowedRttWidth = canvasSize[0];
      var windowedRttHeight = canvasSize[1];
  
      if (strategy.scaleMode == 3) {
        setLetterbox(target, (cssHeight - windowedCssHeight) / 2, (cssWidth - windowedCssWidth) / 2);
        cssWidth = windowedCssWidth;
        cssHeight = windowedCssHeight;
      } else if (strategy.scaleMode == 2) {
        if (cssWidth*windowedRttHeight < windowedRttWidth*cssHeight) {
          var desiredCssHeight = windowedRttHeight * cssWidth / windowedRttWidth;
          setLetterbox(target, (cssHeight - desiredCssHeight) / 2, 0);
          cssHeight = desiredCssHeight;
        } else {
          var desiredCssWidth = windowedRttWidth * cssHeight / windowedRttHeight;
          setLetterbox(target, 0, (cssWidth - desiredCssWidth) / 2);
          cssWidth = desiredCssWidth;
        }
      }
  
      // If we are adding padding, must choose a background color or otherwise Chrome will give the
      // padding a default white color. Do it only if user has not customized their own background color.
      if (!target.style.backgroundColor) target.style.backgroundColor = 'black';
      // IE11 does the same, but requires the color to be set in the document body.
      if (!document.body.style.backgroundColor) document.body.style.backgroundColor = 'black'; // IE11
      // Firefox always shows black letterboxes independent of style color.
  
      target.style.width = cssWidth + 'px';
      target.style.height = cssHeight + 'px';
  
      if (strategy.filteringMode == 1) {
        target.style.imageRendering = 'optimizeSpeed';
        target.style.imageRendering = '-moz-crisp-edges';
        target.style.imageRendering = '-o-crisp-edges';
        target.style.imageRendering = '-webkit-optimize-contrast';
        target.style.imageRendering = 'optimize-contrast';
        target.style.imageRendering = 'crisp-edges';
        target.style.imageRendering = 'pixelated';
      }
  
      var dpiScale = (strategy.canvasResolutionScaleMode == 2) ? devicePixelRatio : 1;
      if (strategy.canvasResolutionScaleMode != 0) {
        var newWidth = (cssWidth * dpiScale)|0;
        var newHeight = (cssHeight * dpiScale)|0;
        setCanvasElementSize(target, newWidth, newHeight);
        if (target.GLctxObject) target.GLctxObject.GLctx.viewport(0, 0, newWidth, newHeight);
      }
      return restoreOldStyle;
    }
  
  function JSEvents_requestFullscreen(target, strategy) {
      // EMSCRIPTEN_FULLSCREEN_SCALE_DEFAULT + EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_NONE is a mode where no extra logic is performed to the DOM elements.
      if (strategy.scaleMode != 0 || strategy.canvasResolutionScaleMode != 0) {
        JSEvents_resizeCanvasForFullscreen(target, strategy);
      }
  
      if (target.requestFullscreen) {
        target.requestFullscreen();
      } else if (target.webkitRequestFullscreen) {
        target.webkitRequestFullscreen(Element.ALLOW_KEYBOARD_INPUT);
      } else {
        return JSEvents.fullscreenEnabled() ? -3 : -1;
      }
  
      currentFullscreenStrategy = strategy;
  
      if (strategy.canvasResizedCallback) {
        getWasmTableEntry(strategy.canvasResizedCallback)(37, 0, strategy.canvasResizedCallbackUserData);
      }
  
      return 0;
    }
  
  function _emscripten_exit_fullscreen() {
      if (!JSEvents.fullscreenEnabled()) return -1;
      // Make sure no queued up calls will fire after this.
      JSEvents.removeDeferredCalls(JSEvents_requestFullscreen);
  
      var d = specialHTMLTargets[1];
      if (d.exitFullscreen) {
        d.fullscreenElement && d.exitFullscreen();
      } else if (d.webkitExitFullscreen) {
        d.webkitFullscreenElement && d.webkitExitFullscreen();
      } else {
        return -1;
      }
  
      return 0;
    }

  
  function requestPointerLock(target) {
      if (target.requestPointerLock) {
        target.requestPointerLock();
      } else {
        // document.body is known to accept pointer lock, so use that to differentiate if the user passed a bad element,
        // or if the whole browser just doesn't support the feature.
        if (document.body.requestPointerLock
          ) {
          return -3;
        }
        return -1;
      }
      return 0;
    }
  function _emscripten_exit_pointerlock() {
      // Make sure no queued up calls will fire after this.
      JSEvents.removeDeferredCalls(requestPointerLock);
  
      if (document.exitPointerLock) {
        document.exitPointerLock();
      } else {
        return -1;
      }
      return 0;
    }

  function _emscripten_get_device_pixel_ratio() {
      return (typeof devicePixelRatio == 'number' && devicePixelRatio) || 1.0;
    }

  
  
  function _emscripten_get_element_css_size(target, width, height) {
      target = findEventTarget(target);
      if (!target) return -4;
  
      var rect = getBoundingClientRect(target);
      HEAPF64[((width)>>3)] = rect.width;
      HEAPF64[((height)>>3)] = rect.height;
  
      return 0;
    }

  
  function fillGamepadEventData(eventStruct, e) {
      HEAPF64[((eventStruct)>>3)] = e.timestamp;
      for (var i = 0; i < e.axes.length; ++i) {
        HEAPF64[(((eventStruct+i*8)+(16))>>3)] = e.axes[i];
      }
      for (var i = 0; i < e.buttons.length; ++i) {
        if (typeof e.buttons[i] == 'object') {
          HEAPF64[(((eventStruct+i*8)+(528))>>3)] = e.buttons[i].value;
        } else {
          HEAPF64[(((eventStruct+i*8)+(528))>>3)] = e.buttons[i];
        }
      }
      for (var i = 0; i < e.buttons.length; ++i) {
        if (typeof e.buttons[i] == 'object') {
          HEAP32[(((eventStruct+i*4)+(1040))>>2)] = e.buttons[i].pressed;
        } else {
          // Assigning a boolean to HEAP32, that's ok, but Closure would like to warn about it:
          /** @suppress {checkTypes} */
          HEAP32[(((eventStruct+i*4)+(1040))>>2)] = e.buttons[i] == 1;
        }
      }
      HEAP32[(((eventStruct)+(1296))>>2)] = e.connected;
      HEAP32[(((eventStruct)+(1300))>>2)] = e.index;
      HEAP32[(((eventStruct)+(8))>>2)] = e.axes.length;
      HEAP32[(((eventStruct)+(12))>>2)] = e.buttons.length;
      stringToUTF8(e.id, eventStruct + 1304, 64);
      stringToUTF8(e.mapping, eventStruct + 1368, 64);
    }
  function _emscripten_get_gamepad_status(index, gamepadState) {
      if (!JSEvents.lastGamepadState) throw 'emscripten_get_gamepad_status() can only be called after having first called emscripten_sample_gamepad_data() and that function has returned EMSCRIPTEN_RESULT_SUCCESS!';
  
      // INVALID_PARAM is returned on a Gamepad index that never was there.
      if (index < 0 || index >= JSEvents.lastGamepadState.length) return -5;
  
      // NO_DATA is returned on a Gamepad index that was removed.
      // For previously disconnected gamepads there should be an empty slot (null/undefined/false) at the index.
      // This is because gamepads must keep their original position in the array.
      // For example, removing the first of two gamepads produces [null/undefined/false, gamepad].
      if (!JSEvents.lastGamepadState[index]) return -7;
  
      fillGamepadEventData(gamepadState, JSEvents.lastGamepadState[index]);
      return 0;
    }

  var getHeapMax = () =>
      // Stay one Wasm page short of 4GB: while e.g. Chrome is able to allocate
      // full 4GB Wasm memories, the size will wrap back to 0 bytes in Wasm side
      // for any code that deals with heap sizes, which would require special
      // casing all heap size related code to treat 0 specially.
      2147483648;
  var _emscripten_get_heap_max = () => getHeapMax();


  function _emscripten_get_num_gamepads() {
      if (!JSEvents.lastGamepadState) throw 'emscripten_get_num_gamepads() can only be called after having first called emscripten_sample_gamepad_data() and that function has returned EMSCRIPTEN_RESULT_SUCCESS!';
      // N.B. Do not call emscripten_get_num_gamepads() unless having first called emscripten_sample_gamepad_data(), and that has returned EMSCRIPTEN_RESULT_SUCCESS.
      // Otherwise the following line will throw an exception.
      return JSEvents.lastGamepadState.length;
    }

  
  
  
  function _emscripten_get_preloaded_image_data(path, w, h) {
      if ((path | 0) === path) path = UTF8ToString(path);
  
      path = PATH_FS.resolve(path);
  
      var canvas = /** @type {HTMLCanvasElement} */(preloadedImages[path]);
      if (canvas) {
        var ctx = canvas.getContext("2d");
        var image = ctx.getImageData(0, 0, canvas.width, canvas.height);
        var buf = _malloc(canvas.width * canvas.height * 4);
  
        HEAPU8.set(image.data, buf);
  
        HEAP32[((w)>>2)] = canvas.width;
        HEAP32[((h)>>2)] = canvas.height;
        return buf;
      }
  
      return 0;
    }

  
  
  function _emscripten_get_preloaded_image_data_from_FILE(file, w, h) {
      var fd = _fileno(file);
      var stream = FS.getStream(fd);
      if (stream) {
        return _emscripten_get_preloaded_image_data(stream.path, w, h);
      }
  
      return 0;
    }

  function _emscripten_get_screen_size(width, height) {
      HEAP32[((width)>>2)] = screen.width;
      HEAP32[((height)>>2)] = screen.height;
    }

  /** @suppress {duplicate } */
  function _glActiveTexture(x0) { GLctx.activeTexture(x0) }
  var _emscripten_glActiveTexture = _glActiveTexture;

  /** @suppress {duplicate } */
  function _glAttachShader(program, shader) {
      GLctx.attachShader(GL.programs[program], GL.shaders[shader]);
    }
  var _emscripten_glAttachShader = _glAttachShader;

  /** @suppress {duplicate } */
  function _glBeginQuery(target, id) {
      GLctx.beginQuery(target, GL.queries[id]);
    }
  var _emscripten_glBeginQuery = _glBeginQuery;

  /** @suppress {duplicate } */
  function _glBeginQueryEXT(target, id) {
      GLctx.disjointTimerQueryExt['beginQueryEXT'](target, GL.queries[id]);
    }
  var _emscripten_glBeginQueryEXT = _glBeginQueryEXT;

  /** @suppress {duplicate } */
  function _glBeginTransformFeedback(x0) { GLctx.beginTransformFeedback(x0) }
  var _emscripten_glBeginTransformFeedback = _glBeginTransformFeedback;

  
  /** @suppress {duplicate } */
  function _glBindAttribLocation(program, index, name) {
      GLctx.bindAttribLocation(GL.programs[program], index, UTF8ToString(name));
    }
  var _emscripten_glBindAttribLocation = _glBindAttribLocation;

  /** @suppress {duplicate } */
  function _glBindBuffer(target, buffer) {
      if (target == 0x8892 /*GL_ARRAY_BUFFER*/) {
        GLctx.currentArrayBufferBinding = buffer;
      } else if (target == 0x8893 /*GL_ELEMENT_ARRAY_BUFFER*/) {
        GLctx.currentElementArrayBufferBinding = buffer;
      }
  
      if (target == 0x88EB /*GL_PIXEL_PACK_BUFFER*/) {
        // In WebGL 2 glReadPixels entry point, we need to use a different WebGL 2 API function call when a buffer is bound to
        // GL_PIXEL_PACK_BUFFER_BINDING point, so must keep track whether that binding point is non-null to know what is
        // the proper API function to call.
        GLctx.currentPixelPackBufferBinding = buffer;
      } else if (target == 0x88EC /*GL_PIXEL_UNPACK_BUFFER*/) {
        // In WebGL 2 gl(Compressed)Tex(Sub)Image[23]D entry points, we need to
        // use a different WebGL 2 API function call when a buffer is bound to
        // GL_PIXEL_UNPACK_BUFFER_BINDING point, so must keep track whether that
        // binding point is non-null to know what is the proper API function to
        // call.
        GLctx.currentPixelUnpackBufferBinding = buffer;
      }
      GLctx.bindBuffer(target, GL.buffers[buffer]);
    }
  var _emscripten_glBindBuffer = _glBindBuffer;

  /** @suppress {duplicate } */
  function _glBindBufferBase(target, index, buffer) {
      GLctx.bindBufferBase(target, index, GL.buffers[buffer]);
    }
  var _emscripten_glBindBufferBase = _glBindBufferBase;

  /** @suppress {duplicate } */
  function _glBindBufferRange(target, index, buffer, offset, ptrsize) {
      GLctx.bindBufferRange(target, index, GL.buffers[buffer], offset, ptrsize);
    }
  var _emscripten_glBindBufferRange = _glBindBufferRange;

  /** @suppress {duplicate } */
  function _glBindFramebuffer(target, framebuffer) {
  
      GLctx.bindFramebuffer(target, GL.framebuffers[framebuffer]);
  
    }
  var _emscripten_glBindFramebuffer = _glBindFramebuffer;

  /** @suppress {duplicate } */
  function _glBindRenderbuffer(target, renderbuffer) {
      GLctx.bindRenderbuffer(target, GL.renderbuffers[renderbuffer]);
    }
  var _emscripten_glBindRenderbuffer = _glBindRenderbuffer;

  /** @suppress {duplicate } */
  function _glBindSampler(unit, sampler) {
      GLctx.bindSampler(unit, GL.samplers[sampler]);
    }
  var _emscripten_glBindSampler = _glBindSampler;

  /** @suppress {duplicate } */
  function _glBindTexture(target, texture) {
      GLctx.bindTexture(target, GL.textures[texture]);
    }
  var _emscripten_glBindTexture = _glBindTexture;

  /** @suppress {duplicate } */
  function _glBindTransformFeedback(target, id) {
      GLctx.bindTransformFeedback(target, GL.transformFeedbacks[id]);
    }
  var _emscripten_glBindTransformFeedback = _glBindTransformFeedback;

  /** @suppress {duplicate } */
  function _glBindVertexArray(vao) {
      GLctx.bindVertexArray(GL.vaos[vao]);
      var ibo = GLctx.getParameter(0x8895 /*ELEMENT_ARRAY_BUFFER_BINDING*/);
      GLctx.currentElementArrayBufferBinding = ibo ? (ibo.name | 0) : 0;
    }
  var _emscripten_glBindVertexArray = _glBindVertexArray;

  
  /** @suppress {duplicate } */
  var _glBindVertexArrayOES = _glBindVertexArray;
  var _emscripten_glBindVertexArrayOES = _glBindVertexArrayOES;

  /** @suppress {duplicate } */
  function _glBlendColor(x0, x1, x2, x3) { GLctx.blendColor(x0, x1, x2, x3) }
  var _emscripten_glBlendColor = _glBlendColor;

  /** @suppress {duplicate } */
  function _glBlendEquation(x0) { GLctx.blendEquation(x0) }
  var _emscripten_glBlendEquation = _glBlendEquation;

  /** @suppress {duplicate } */
  function _glBlendEquationSeparate(x0, x1) { GLctx.blendEquationSeparate(x0, x1) }
  var _emscripten_glBlendEquationSeparate = _glBlendEquationSeparate;

  /** @suppress {duplicate } */
  function _glBlendFunc(x0, x1) { GLctx.blendFunc(x0, x1) }
  var _emscripten_glBlendFunc = _glBlendFunc;

  /** @suppress {duplicate } */
  function _glBlendFuncSeparate(x0, x1, x2, x3) { GLctx.blendFuncSeparate(x0, x1, x2, x3) }
  var _emscripten_glBlendFuncSeparate = _glBlendFuncSeparate;

  /** @suppress {duplicate } */
  function _glBlitFramebuffer(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9) { GLctx.blitFramebuffer(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9) }
  var _emscripten_glBlitFramebuffer = _glBlitFramebuffer;

  /** @suppress {duplicate } */
  function _glBufferData(target, size, data, usage) {
  
      if (GL.currentContext.version >= 2) { // WebGL 2 provides new garbage-free entry points to call to WebGL. Use those always when possible.
        // If size is zero, WebGL would interpret uploading the whole input arraybuffer (starting from given offset), which would
        // not make sense in WebAssembly, so avoid uploading if size is zero. However we must still call bufferData to establish a
        // backing storage of zero bytes.
        if (data && size) {
          GLctx.bufferData(target, HEAPU8, usage, data, size);
        } else {
          GLctx.bufferData(target, size, usage);
        }
      } else {
        // N.b. here first form specifies a heap subarray, second form an integer size, so the ?: code here is polymorphic. It is advised to avoid
        // randomly mixing both uses in calling code, to avoid any potential JS engine JIT issues.
        GLctx.bufferData(target, data ? HEAPU8.subarray(data, data+size) : size, usage);
      }
    }
  var _emscripten_glBufferData = _glBufferData;

  /** @suppress {duplicate } */
  function _glBufferSubData(target, offset, size, data) {
      if (GL.currentContext.version >= 2) { // WebGL 2 provides new garbage-free entry points to call to WebGL. Use those always when possible.
        size && GLctx.bufferSubData(target, offset, HEAPU8, data, size);
        return;
      }
      GLctx.bufferSubData(target, offset, HEAPU8.subarray(data, data+size));
    }
  var _emscripten_glBufferSubData = _glBufferSubData;

  /** @suppress {duplicate } */
  function _glCheckFramebufferStatus(x0) { return GLctx.checkFramebufferStatus(x0) }
  var _emscripten_glCheckFramebufferStatus = _glCheckFramebufferStatus;

  /** @suppress {duplicate } */
  function _glClear(x0) { GLctx.clear(x0) }
  var _emscripten_glClear = _glClear;

  /** @suppress {duplicate } */
  function _glClearBufferfi(x0, x1, x2, x3) { GLctx.clearBufferfi(x0, x1, x2, x3) }
  var _emscripten_glClearBufferfi = _glClearBufferfi;

  /** @suppress {duplicate } */
  function _glClearBufferfv(buffer, drawbuffer, value) {
  
      GLctx.clearBufferfv(buffer, drawbuffer, HEAPF32, value>>2);
    }
  var _emscripten_glClearBufferfv = _glClearBufferfv;

  /** @suppress {duplicate } */
  function _glClearBufferiv(buffer, drawbuffer, value) {
  
      GLctx.clearBufferiv(buffer, drawbuffer, HEAP32, value>>2);
    }
  var _emscripten_glClearBufferiv = _glClearBufferiv;

  /** @suppress {duplicate } */
  function _glClearBufferuiv(buffer, drawbuffer, value) {
  
      GLctx.clearBufferuiv(buffer, drawbuffer, HEAPU32, value>>2);
    }
  var _emscripten_glClearBufferuiv = _glClearBufferuiv;

  /** @suppress {duplicate } */
  function _glClearColor(x0, x1, x2, x3) { GLctx.clearColor(x0, x1, x2, x3) }
  var _emscripten_glClearColor = _glClearColor;

  /** @suppress {duplicate } */
  function _glClearDepthf(x0) { GLctx.clearDepth(x0) }
  var _emscripten_glClearDepthf = _glClearDepthf;

  /** @suppress {duplicate } */
  function _glClearStencil(x0) { GLctx.clearStencil(x0) }
  var _emscripten_glClearStencil = _glClearStencil;

  function convertI32PairToI53(lo, hi) {
      // This function should not be getting called with too large unsigned numbers
      // in high part (if hi >= 0x7FFFFFFFF, one should have been calling
      // convertU32PairToI53())
      assert(hi === (hi|0));
      return (lo >>> 0) + hi * 4294967296;
    }
  /** @suppress {duplicate } */
  function _glClientWaitSync(sync, flags, timeout_low, timeout_high) {
      // WebGL2 vs GLES3 differences: in GLES3, the timeout parameter is a uint64, where 0xFFFFFFFFFFFFFFFFULL means GL_TIMEOUT_IGNORED.
      // In JS, there's no 64-bit value types, so instead timeout is taken to be signed, and GL_TIMEOUT_IGNORED is given value -1.
      // Inherently the value accepted in the timeout is lossy, and can't take in arbitrary u64 bit pattern (but most likely doesn't matter)
      // See https://www.khronos.org/registry/webgl/specs/latest/2.0/#5.15
      var timeout = convertI32PairToI53(timeout_low, timeout_high);
      return GLctx.clientWaitSync(GL.syncs[sync], flags, timeout);
    }
  var _emscripten_glClientWaitSync = _glClientWaitSync;

  /** @suppress {duplicate } */
  function _glColorMask(red, green, blue, alpha) {
      GLctx.colorMask(!!red, !!green, !!blue, !!alpha);
    }
  var _emscripten_glColorMask = _glColorMask;

  /** @suppress {duplicate } */
  function _glCompileShader(shader) {
      GLctx.compileShader(GL.shaders[shader]);
    }
  var _emscripten_glCompileShader = _glCompileShader;

  /** @suppress {duplicate } */
  function _glCompressedTexImage2D(target, level, internalFormat, width, height, border, imageSize, data) {
      if (GL.currentContext.version >= 2) { // WebGL 2 provides new garbage-free entry points to call to WebGL. Use those always when possible.
        if (GLctx.currentPixelUnpackBufferBinding || !imageSize) {
          GLctx.compressedTexImage2D(target, level, internalFormat, width, height, border, imageSize, data);
        } else {
          GLctx.compressedTexImage2D(target, level, internalFormat, width, height, border, HEAPU8, data, imageSize);
        }
        return;
      }
      GLctx.compressedTexImage2D(target, level, internalFormat, width, height, border, data ? HEAPU8.subarray((data), (data+imageSize)) : null);
    }
  var _emscripten_glCompressedTexImage2D = _glCompressedTexImage2D;

  /** @suppress {duplicate } */
  function _glCompressedTexImage3D(target, level, internalFormat, width, height, depth, border, imageSize, data) {
      if (GLctx.currentPixelUnpackBufferBinding) {
        GLctx.compressedTexImage3D(target, level, internalFormat, width, height, depth, border, imageSize, data);
      } else {
        GLctx.compressedTexImage3D(target, level, internalFormat, width, height, depth, border, HEAPU8, data, imageSize);
      }
    }
  var _emscripten_glCompressedTexImage3D = _glCompressedTexImage3D;

  /** @suppress {duplicate } */
  function _glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data) {
      if (GL.currentContext.version >= 2) { // WebGL 2 provides new garbage-free entry points to call to WebGL. Use those always when possible.
        if (GLctx.currentPixelUnpackBufferBinding || !imageSize) {
          GLctx.compressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data);
        } else {
          GLctx.compressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, HEAPU8, data, imageSize);
        }
        return;
      }
      GLctx.compressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, data ? HEAPU8.subarray((data), (data+imageSize)) : null);
    }
  var _emscripten_glCompressedTexSubImage2D = _glCompressedTexSubImage2D;

  /** @suppress {duplicate } */
  function _glCompressedTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data) {
      if (GLctx.currentPixelUnpackBufferBinding) {
        GLctx.compressedTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data);
      } else {
        GLctx.compressedTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, HEAPU8, data, imageSize);
      }
    }
  var _emscripten_glCompressedTexSubImage3D = _glCompressedTexSubImage3D;

  /** @suppress {duplicate } */
  function _glCopyBufferSubData(x0, x1, x2, x3, x4) { GLctx.copyBufferSubData(x0, x1, x2, x3, x4) }
  var _emscripten_glCopyBufferSubData = _glCopyBufferSubData;

  /** @suppress {duplicate } */
  function _glCopyTexImage2D(x0, x1, x2, x3, x4, x5, x6, x7) { GLctx.copyTexImage2D(x0, x1, x2, x3, x4, x5, x6, x7) }
  var _emscripten_glCopyTexImage2D = _glCopyTexImage2D;

  /** @suppress {duplicate } */
  function _glCopyTexSubImage2D(x0, x1, x2, x3, x4, x5, x6, x7) { GLctx.copyTexSubImage2D(x0, x1, x2, x3, x4, x5, x6, x7) }
  var _emscripten_glCopyTexSubImage2D = _glCopyTexSubImage2D;

  /** @suppress {duplicate } */
  function _glCopyTexSubImage3D(x0, x1, x2, x3, x4, x5, x6, x7, x8) { GLctx.copyTexSubImage3D(x0, x1, x2, x3, x4, x5, x6, x7, x8) }
  var _emscripten_glCopyTexSubImage3D = _glCopyTexSubImage3D;

  /** @suppress {duplicate } */
  function _glCreateProgram() {
      var id = GL.getNewId(GL.programs);
      var program = GLctx.createProgram();
      // Store additional information needed for each shader program:
      program.name = id;
      // Lazy cache results of glGetProgramiv(GL_ACTIVE_UNIFORM_MAX_LENGTH/GL_ACTIVE_ATTRIBUTE_MAX_LENGTH/GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH)
      program.maxUniformLength = program.maxAttributeLength = program.maxUniformBlockNameLength = 0;
      program.uniformIdCounter = 1;
      GL.programs[id] = program;
      return id;
    }
  var _emscripten_glCreateProgram = _glCreateProgram;

  /** @suppress {duplicate } */
  function _glCreateShader(shaderType) {
      var id = GL.getNewId(GL.shaders);
      GL.shaders[id] = GLctx.createShader(shaderType);
  
      return id;
    }
  var _emscripten_glCreateShader = _glCreateShader;

  /** @suppress {duplicate } */
  function _glCullFace(x0) { GLctx.cullFace(x0) }
  var _emscripten_glCullFace = _glCullFace;

  /** @suppress {duplicate } */
  function _glDeleteBuffers(n, buffers) {
      for (var i = 0; i < n; i++) {
        var id = HEAP32[(((buffers)+(i*4))>>2)];
        var buffer = GL.buffers[id];
  
        // From spec: "glDeleteBuffers silently ignores 0's and names that do not
        // correspond to existing buffer objects."
        if (!buffer) continue;
  
        GLctx.deleteBuffer(buffer);
        buffer.name = 0;
        GL.buffers[id] = null;
  
        if (id == GLctx.currentArrayBufferBinding) GLctx.currentArrayBufferBinding = 0;
        if (id == GLctx.currentElementArrayBufferBinding) GLctx.currentElementArrayBufferBinding = 0;
        if (id == GLctx.currentPixelPackBufferBinding) GLctx.currentPixelPackBufferBinding = 0;
        if (id == GLctx.currentPixelUnpackBufferBinding) GLctx.currentPixelUnpackBufferBinding = 0;
      }
    }
  var _emscripten_glDeleteBuffers = _glDeleteBuffers;

  /** @suppress {duplicate } */
  function _glDeleteFramebuffers(n, framebuffers) {
      for (var i = 0; i < n; ++i) {
        var id = HEAP32[(((framebuffers)+(i*4))>>2)];
        var framebuffer = GL.framebuffers[id];
        if (!framebuffer) continue; // GL spec: "glDeleteFramebuffers silently ignores 0s and names that do not correspond to existing framebuffer objects".
        GLctx.deleteFramebuffer(framebuffer);
        framebuffer.name = 0;
        GL.framebuffers[id] = null;
      }
    }
  var _emscripten_glDeleteFramebuffers = _glDeleteFramebuffers;

  /** @suppress {duplicate } */
  function _glDeleteProgram(id) {
      if (!id) return;
      var program = GL.programs[id];
      if (!program) { // glDeleteProgram actually signals an error when deleting a nonexisting object, unlike some other GL delete functions.
        GL.recordError(0x501 /* GL_INVALID_VALUE */);
        return;
      }
      GLctx.deleteProgram(program);
      program.name = 0;
      GL.programs[id] = null;
    }
  var _emscripten_glDeleteProgram = _glDeleteProgram;

  /** @suppress {duplicate } */
  function _glDeleteQueries(n, ids) {
      for (var i = 0; i < n; i++) {
        var id = HEAP32[(((ids)+(i*4))>>2)];
        var query = GL.queries[id];
        if (!query) continue; // GL spec: "unused names in ids are ignored, as is the name zero."
        GLctx.deleteQuery(query);
        GL.queries[id] = null;
      }
    }
  var _emscripten_glDeleteQueries = _glDeleteQueries;

  /** @suppress {duplicate } */
  function _glDeleteQueriesEXT(n, ids) {
      for (var i = 0; i < n; i++) {
        var id = HEAP32[(((ids)+(i*4))>>2)];
        var query = GL.queries[id];
        if (!query) continue; // GL spec: "unused names in ids are ignored, as is the name zero."
        GLctx.disjointTimerQueryExt['deleteQueryEXT'](query);
        GL.queries[id] = null;
      }
    }
  var _emscripten_glDeleteQueriesEXT = _glDeleteQueriesEXT;

  /** @suppress {duplicate } */
  function _glDeleteRenderbuffers(n, renderbuffers) {
      for (var i = 0; i < n; i++) {
        var id = HEAP32[(((renderbuffers)+(i*4))>>2)];
        var renderbuffer = GL.renderbuffers[id];
        if (!renderbuffer) continue; // GL spec: "glDeleteRenderbuffers silently ignores 0s and names that do not correspond to existing renderbuffer objects".
        GLctx.deleteRenderbuffer(renderbuffer);
        renderbuffer.name = 0;
        GL.renderbuffers[id] = null;
      }
    }
  var _emscripten_glDeleteRenderbuffers = _glDeleteRenderbuffers;

  /** @suppress {duplicate } */
  function _glDeleteSamplers(n, samplers) {
      for (var i = 0; i < n; i++) {
        var id = HEAP32[(((samplers)+(i*4))>>2)];
        var sampler = GL.samplers[id];
        if (!sampler) continue;
        GLctx.deleteSampler(sampler);
        sampler.name = 0;
        GL.samplers[id] = null;
      }
    }
  var _emscripten_glDeleteSamplers = _glDeleteSamplers;

  /** @suppress {duplicate } */
  function _glDeleteShader(id) {
      if (!id) return;
      var shader = GL.shaders[id];
      if (!shader) { // glDeleteShader actually signals an error when deleting a nonexisting object, unlike some other GL delete functions.
        GL.recordError(0x501 /* GL_INVALID_VALUE */);
        return;
      }
      GLctx.deleteShader(shader);
      GL.shaders[id] = null;
    }
  var _emscripten_glDeleteShader = _glDeleteShader;

  /** @suppress {duplicate } */
  function _glDeleteSync(id) {
      if (!id) return;
      var sync = GL.syncs[id];
      if (!sync) { // glDeleteSync signals an error when deleting a nonexisting object, unlike some other GL delete functions.
        GL.recordError(0x501 /* GL_INVALID_VALUE */);
        return;
      }
      GLctx.deleteSync(sync);
      sync.name = 0;
      GL.syncs[id] = null;
    }
  var _emscripten_glDeleteSync = _glDeleteSync;

  /** @suppress {duplicate } */
  function _glDeleteTextures(n, textures) {
      for (var i = 0; i < n; i++) {
        var id = HEAP32[(((textures)+(i*4))>>2)];
        var texture = GL.textures[id];
        if (!texture) continue; // GL spec: "glDeleteTextures silently ignores 0s and names that do not correspond to existing textures".
        GLctx.deleteTexture(texture);
        texture.name = 0;
        GL.textures[id] = null;
      }
    }
  var _emscripten_glDeleteTextures = _glDeleteTextures;

  /** @suppress {duplicate } */
  function _glDeleteTransformFeedbacks(n, ids) {
      for (var i = 0; i < n; i++) {
        var id = HEAP32[(((ids)+(i*4))>>2)];
        var transformFeedback = GL.transformFeedbacks[id];
        if (!transformFeedback) continue; // GL spec: "unused names in ids are ignored, as is the name zero."
        GLctx.deleteTransformFeedback(transformFeedback);
        transformFeedback.name = 0;
        GL.transformFeedbacks[id] = null;
      }
    }
  var _emscripten_glDeleteTransformFeedbacks = _glDeleteTransformFeedbacks;

  /** @suppress {duplicate } */
  function _glDeleteVertexArrays(n, vaos) {
      for (var i = 0; i < n; i++) {
        var id = HEAP32[(((vaos)+(i*4))>>2)];
        GLctx.deleteVertexArray(GL.vaos[id]);
        GL.vaos[id] = null;
      }
    }
  var _emscripten_glDeleteVertexArrays = _glDeleteVertexArrays;

  
  /** @suppress {duplicate } */
  var _glDeleteVertexArraysOES = _glDeleteVertexArrays;
  var _emscripten_glDeleteVertexArraysOES = _glDeleteVertexArraysOES;

  /** @suppress {duplicate } */
  function _glDepthFunc(x0) { GLctx.depthFunc(x0) }
  var _emscripten_glDepthFunc = _glDepthFunc;

  /** @suppress {duplicate } */
  function _glDepthMask(flag) {
      GLctx.depthMask(!!flag);
    }
  var _emscripten_glDepthMask = _glDepthMask;

  /** @suppress {duplicate } */
  function _glDepthRangef(x0, x1) { GLctx.depthRange(x0, x1) }
  var _emscripten_glDepthRangef = _glDepthRangef;

  /** @suppress {duplicate } */
  function _glDetachShader(program, shader) {
      GLctx.detachShader(GL.programs[program], GL.shaders[shader]);
    }
  var _emscripten_glDetachShader = _glDetachShader;

  /** @suppress {duplicate } */
  function _glDisable(x0) { GLctx.disable(x0) }
  var _emscripten_glDisable = _glDisable;

  /** @suppress {duplicate } */
  function _glDisableVertexAttribArray(index) {
      var cb = GL.currentContext.clientBuffers[index];
      cb.enabled = false;
      GLctx.disableVertexAttribArray(index);
    }
  var _emscripten_glDisableVertexAttribArray = _glDisableVertexAttribArray;

  /** @suppress {duplicate } */
  function _glDrawArrays(mode, first, count) {
      // bind any client-side buffers
      GL.preDrawHandleClientVertexAttribBindings(first + count);
  
      GLctx.drawArrays(mode, first, count);
  
      GL.postDrawHandleClientVertexAttribBindings();
    }
  var _emscripten_glDrawArrays = _glDrawArrays;

  /** @suppress {duplicate } */
  function _glDrawArraysInstanced(mode, first, count, primcount) {
      GLctx.drawArraysInstanced(mode, first, count, primcount);
    }
  var _emscripten_glDrawArraysInstanced = _glDrawArraysInstanced;

  
  /** @suppress {duplicate } */
  var _glDrawArraysInstancedANGLE = _glDrawArraysInstanced;
  var _emscripten_glDrawArraysInstancedANGLE = _glDrawArraysInstancedANGLE;

  
  /** @suppress {duplicate } */
  var _glDrawArraysInstancedARB = _glDrawArraysInstanced;
  var _emscripten_glDrawArraysInstancedARB = _glDrawArraysInstancedARB;

  
  /** @suppress {duplicate } */
  var _glDrawArraysInstancedEXT = _glDrawArraysInstanced;
  var _emscripten_glDrawArraysInstancedEXT = _glDrawArraysInstancedEXT;

  
  /** @suppress {duplicate } */
  var _glDrawArraysInstancedNV = _glDrawArraysInstanced;
  var _emscripten_glDrawArraysInstancedNV = _glDrawArraysInstancedNV;

  var tempFixedLengthArray = [];
  
  /** @suppress {duplicate } */
  function _glDrawBuffers(n, bufs) {
  
      var bufArray = tempFixedLengthArray[n];
      for (var i = 0; i < n; i++) {
        bufArray[i] = HEAP32[(((bufs)+(i*4))>>2)];
      }
  
      GLctx.drawBuffers(bufArray);
    }
  var _emscripten_glDrawBuffers = _glDrawBuffers;

  
  /** @suppress {duplicate } */
  var _glDrawBuffersEXT = _glDrawBuffers;
  var _emscripten_glDrawBuffersEXT = _glDrawBuffersEXT;

  
  /** @suppress {duplicate } */
  var _glDrawBuffersWEBGL = _glDrawBuffers;
  var _emscripten_glDrawBuffersWEBGL = _glDrawBuffersWEBGL;

  /** @suppress {duplicate } */
  function _glDrawElements(mode, count, type, indices) {
      var buf;
      if (!GLctx.currentElementArrayBufferBinding) {
        var size = GL.calcBufLength(1, type, 0, count);
        buf = GL.getTempIndexBuffer(size);
        GLctx.bindBuffer(0x8893 /*GL_ELEMENT_ARRAY_BUFFER*/, buf);
        GLctx.bufferSubData(0x8893 /*GL_ELEMENT_ARRAY_BUFFER*/,
                                 0,
                                 HEAPU8.subarray(indices, indices + size));
        // the index is now 0
        indices = 0;
      }
  
      // bind any client-side buffers
      GL.preDrawHandleClientVertexAttribBindings(count);
  
      GLctx.drawElements(mode, count, type, indices);
  
      GL.postDrawHandleClientVertexAttribBindings(count);
  
      if (!GLctx.currentElementArrayBufferBinding) {
        GLctx.bindBuffer(0x8893 /*GL_ELEMENT_ARRAY_BUFFER*/, null);
      }
    }
  var _emscripten_glDrawElements = _glDrawElements;

  /** @suppress {duplicate } */
  function _glDrawElementsInstanced(mode, count, type, indices, primcount) {
      GLctx.drawElementsInstanced(mode, count, type, indices, primcount);
    }
  var _emscripten_glDrawElementsInstanced = _glDrawElementsInstanced;

  
  /** @suppress {duplicate } */
  var _glDrawElementsInstancedANGLE = _glDrawElementsInstanced;
  var _emscripten_glDrawElementsInstancedANGLE = _glDrawElementsInstancedANGLE;

  
  /** @suppress {duplicate } */
  var _glDrawElementsInstancedARB = _glDrawElementsInstanced;
  var _emscripten_glDrawElementsInstancedARB = _glDrawElementsInstancedARB;

  
  /** @suppress {duplicate } */
  var _glDrawElementsInstancedEXT = _glDrawElementsInstanced;
  var _emscripten_glDrawElementsInstancedEXT = _glDrawElementsInstancedEXT;

  
  /** @suppress {duplicate } */
  var _glDrawElementsInstancedNV = _glDrawElementsInstanced;
  var _emscripten_glDrawElementsInstancedNV = _glDrawElementsInstancedNV;

  /** @suppress {duplicate } */
  function _glDrawRangeElements(mode, start, end, count, type, indices) {
      // TODO: This should be a trivial pass-though function registered at the bottom of this page as
      // glFuncs[6][1] += ' drawRangeElements';
      // but due to https://bugzilla.mozilla.org/show_bug.cgi?id=1202427,
      // we work around by ignoring the range.
      _glDrawElements(mode, count, type, indices);
    }
  var _emscripten_glDrawRangeElements = _glDrawRangeElements;

  /** @suppress {duplicate } */
  function _glEnable(x0) { GLctx.enable(x0) }
  var _emscripten_glEnable = _glEnable;

  /** @suppress {duplicate } */
  function _glEnableVertexAttribArray(index) {
      var cb = GL.currentContext.clientBuffers[index];
      cb.enabled = true;
      GLctx.enableVertexAttribArray(index);
    }
  var _emscripten_glEnableVertexAttribArray = _glEnableVertexAttribArray;

  /** @suppress {duplicate } */
  function _glEndQuery(x0) { GLctx.endQuery(x0) }
  var _emscripten_glEndQuery = _glEndQuery;

  /** @suppress {duplicate } */
  function _glEndQueryEXT(target) {
      GLctx.disjointTimerQueryExt['endQueryEXT'](target);
    }
  var _emscripten_glEndQueryEXT = _glEndQueryEXT;

  /** @suppress {duplicate } */
  function _glEndTransformFeedback() { GLctx.endTransformFeedback() }
  var _emscripten_glEndTransformFeedback = _glEndTransformFeedback;

  /** @suppress {duplicate } */
  function _glFenceSync(condition, flags) {
      var sync = GLctx.fenceSync(condition, flags);
      if (sync) {
        var id = GL.getNewId(GL.syncs);
        sync.name = id;
        GL.syncs[id] = sync;
        return id;
      }
      return 0; // Failed to create a sync object
    }
  var _emscripten_glFenceSync = _glFenceSync;

  /** @suppress {duplicate } */
  function _glFinish() { GLctx.finish() }
  var _emscripten_glFinish = _glFinish;

  /** @suppress {duplicate } */
  function _glFlush() { GLctx.flush() }
  var _emscripten_glFlush = _glFlush;

  function emscriptenWebGLGetBufferBinding(target) {
      switch (target) {
        case 0x8892 /*GL_ARRAY_BUFFER*/: target = 0x8894 /*GL_ARRAY_BUFFER_BINDING*/; break;
        case 0x8893 /*GL_ELEMENT_ARRAY_BUFFER*/: target = 0x8895 /*GL_ELEMENT_ARRAY_BUFFER_BINDING*/; break;
        case 0x88EB /*GL_PIXEL_PACK_BUFFER*/: target = 0x88ED /*GL_PIXEL_PACK_BUFFER_BINDING*/; break;
        case 0x88EC /*GL_PIXEL_UNPACK_BUFFER*/: target = 0x88EF /*GL_PIXEL_UNPACK_BUFFER_BINDING*/; break;
        case 0x8C8E /*GL_TRANSFORM_FEEDBACK_BUFFER*/: target = 0x8C8F /*GL_TRANSFORM_FEEDBACK_BUFFER_BINDING*/; break;
        case 0x8F36 /*GL_COPY_READ_BUFFER*/: target = 0x8F36 /*GL_COPY_READ_BUFFER_BINDING*/; break;
        case 0x8F37 /*GL_COPY_WRITE_BUFFER*/: target = 0x8F37 /*GL_COPY_WRITE_BUFFER_BINDING*/; break;
        case 0x8A11 /*GL_UNIFORM_BUFFER*/: target = 0x8A28 /*GL_UNIFORM_BUFFER_BINDING*/; break;
        // In default case, fall through and assume passed one of the _BINDING enums directly.
      }
      var buffer = GLctx.getParameter(target);
      if (buffer) return buffer.name|0;
      else return 0;
    }
  
  function emscriptenWebGLValidateMapBufferTarget(target) {
      switch (target) {
        case 0x8892: // GL_ARRAY_BUFFER
        case 0x8893: // GL_ELEMENT_ARRAY_BUFFER
        case 0x8F36: // GL_COPY_READ_BUFFER
        case 0x8F37: // GL_COPY_WRITE_BUFFER
        case 0x88EB: // GL_PIXEL_PACK_BUFFER
        case 0x88EC: // GL_PIXEL_UNPACK_BUFFER
        case 0x8C2A: // GL_TEXTURE_BUFFER
        case 0x8C8E: // GL_TRANSFORM_FEEDBACK_BUFFER
        case 0x8A11: // GL_UNIFORM_BUFFER
          return true;
        default:
          return false;
      }
    }
  
  /** @suppress {duplicate } */
  function _glFlushMappedBufferRange(target, offset, length) {
      if (!emscriptenWebGLValidateMapBufferTarget(target)) {
        GL.recordError(0x500/*GL_INVALID_ENUM*/);
        err('GL_INVALID_ENUM in glFlushMappedBufferRange');
        return;
      }
  
      var mapping = GL.mappedBuffers[emscriptenWebGLGetBufferBinding(target)];
      if (!mapping) {
        GL.recordError(0x502 /* GL_INVALID_OPERATION */);
        err('buffer was never mapped in glFlushMappedBufferRange');
        return;
      }
  
      if (!(mapping.access & 0x10)) {
        GL.recordError(0x502 /* GL_INVALID_OPERATION */);
        err('buffer was not mapped with GL_MAP_FLUSH_EXPLICIT_BIT in glFlushMappedBufferRange');
        return;
      }
      if (offset < 0 || length < 0 || offset + length > mapping.length) {
        GL.recordError(0x501 /* GL_INVALID_VALUE */);
        err('invalid range in glFlushMappedBufferRange');
        return;
      }
  
      GLctx.bufferSubData(
        target,
        mapping.offset,
        HEAPU8.subarray(mapping.mem + offset, mapping.mem + offset + length));
    }
  var _emscripten_glFlushMappedBufferRange = _glFlushMappedBufferRange;

  /** @suppress {duplicate } */
  function _glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer) {
      GLctx.framebufferRenderbuffer(target, attachment, renderbuffertarget,
                                         GL.renderbuffers[renderbuffer]);
    }
  var _emscripten_glFramebufferRenderbuffer = _glFramebufferRenderbuffer;

  /** @suppress {duplicate } */
  function _glFramebufferTexture2D(target, attachment, textarget, texture, level) {
      GLctx.framebufferTexture2D(target, attachment, textarget,
                                      GL.textures[texture], level);
    }
  var _emscripten_glFramebufferTexture2D = _glFramebufferTexture2D;

  /** @suppress {duplicate } */
  function _glFramebufferTextureLayer(target, attachment, texture, level, layer) {
      GLctx.framebufferTextureLayer(target, attachment, GL.textures[texture], level, layer);
    }
  var _emscripten_glFramebufferTextureLayer = _glFramebufferTextureLayer;

  /** @suppress {duplicate } */
  function _glFrontFace(x0) { GLctx.frontFace(x0) }
  var _emscripten_glFrontFace = _glFrontFace;

  function __glGenObject(n, buffers, createFunction, objectTable
      ) {
      for (var i = 0; i < n; i++) {
        var buffer = GLctx[createFunction]();
        var id = buffer && GL.getNewId(objectTable);
        if (buffer) {
          buffer.name = id;
          objectTable[id] = buffer;
        } else {
          GL.recordError(0x502 /* GL_INVALID_OPERATION */);
        }
        HEAP32[(((buffers)+(i*4))>>2)] = id;
      }
    }
  
  /** @suppress {duplicate } */
  function _glGenBuffers(n, buffers) {
      __glGenObject(n, buffers, 'createBuffer', GL.buffers
        );
    }
  var _emscripten_glGenBuffers = _glGenBuffers;

  
  /** @suppress {duplicate } */
  function _glGenFramebuffers(n, ids) {
      __glGenObject(n, ids, 'createFramebuffer', GL.framebuffers
        );
    }
  var _emscripten_glGenFramebuffers = _glGenFramebuffers;

  /** @suppress {duplicate } */
  function _glGenQueries(n, ids) {
      __glGenObject(n, ids, 'createQuery', GL.queries
        );
    }
  var _emscripten_glGenQueries = _glGenQueries;

  /** @suppress {duplicate } */
  function _glGenQueriesEXT(n, ids) {
      for (var i = 0; i < n; i++) {
        var query = GLctx.disjointTimerQueryExt['createQueryEXT']();
        if (!query) {
          GL.recordError(0x502 /* GL_INVALID_OPERATION */);
          while (i < n) HEAP32[(((ids)+(i++*4))>>2)] = 0;
          return;
        }
        var id = GL.getNewId(GL.queries);
        query.name = id;
        GL.queries[id] = query;
        HEAP32[(((ids)+(i*4))>>2)] = id;
      }
    }
  var _emscripten_glGenQueriesEXT = _glGenQueriesEXT;

  
  /** @suppress {duplicate } */
  function _glGenRenderbuffers(n, renderbuffers) {
      __glGenObject(n, renderbuffers, 'createRenderbuffer', GL.renderbuffers
        );
    }
  var _emscripten_glGenRenderbuffers = _glGenRenderbuffers;

  /** @suppress {duplicate } */
  function _glGenSamplers(n, samplers) {
      __glGenObject(n, samplers, 'createSampler', GL.samplers
        );
    }
  var _emscripten_glGenSamplers = _glGenSamplers;

  
  /** @suppress {duplicate } */
  function _glGenTextures(n, textures) {
      __glGenObject(n, textures, 'createTexture', GL.textures
        );
    }
  var _emscripten_glGenTextures = _glGenTextures;

  /** @suppress {duplicate } */
  function _glGenTransformFeedbacks(n, ids) {
      __glGenObject(n, ids, 'createTransformFeedback', GL.transformFeedbacks
        );
    }
  var _emscripten_glGenTransformFeedbacks = _glGenTransformFeedbacks;

  
  /** @suppress {duplicate } */
  function _glGenVertexArrays(n, arrays) {
      __glGenObject(n, arrays, 'createVertexArray', GL.vaos
        );
    }
  var _emscripten_glGenVertexArrays = _glGenVertexArrays;

  
  /** @suppress {duplicate } */
  var _glGenVertexArraysOES = _glGenVertexArrays;
  var _emscripten_glGenVertexArraysOES = _glGenVertexArraysOES;

  /** @suppress {duplicate } */
  function _glGenerateMipmap(x0) { GLctx.generateMipmap(x0) }
  var _emscripten_glGenerateMipmap = _glGenerateMipmap;

  
  function __glGetActiveAttribOrUniform(funcName, program, index, bufSize, length, size, type, name) {
      program = GL.programs[program];
      var info = GLctx[funcName](program, index);
      if (info) { // If an error occurs, nothing will be written to length, size and type and name.
        var numBytesWrittenExclNull = name && stringToUTF8(info.name, name, bufSize);
        if (length) HEAP32[((length)>>2)] = numBytesWrittenExclNull;
        if (size) HEAP32[((size)>>2)] = info.size;
        if (type) HEAP32[((type)>>2)] = info.type;
      }
    }
  
  /** @suppress {duplicate } */
  function _glGetActiveAttrib(program, index, bufSize, length, size, type, name) {
      __glGetActiveAttribOrUniform('getActiveAttrib', program, index, bufSize, length, size, type, name);
    }
  var _emscripten_glGetActiveAttrib = _glGetActiveAttrib;

  
  /** @suppress {duplicate } */
  function _glGetActiveUniform(program, index, bufSize, length, size, type, name) {
      __glGetActiveAttribOrUniform('getActiveUniform', program, index, bufSize, length, size, type, name);
    }
  var _emscripten_glGetActiveUniform = _glGetActiveUniform;

  /** @suppress {duplicate } */
  function _glGetActiveUniformBlockName(program, uniformBlockIndex, bufSize, length, uniformBlockName) {
      program = GL.programs[program];
  
      var result = GLctx.getActiveUniformBlockName(program, uniformBlockIndex);
      if (!result) return; // If an error occurs, nothing will be written to uniformBlockName or length.
      if (uniformBlockName && bufSize > 0) {
        var numBytesWrittenExclNull = stringToUTF8(result, uniformBlockName, bufSize);
        if (length) HEAP32[((length)>>2)] = numBytesWrittenExclNull;
      } else {
        if (length) HEAP32[((length)>>2)] = 0;
      }
    }
  var _emscripten_glGetActiveUniformBlockName = _glGetActiveUniformBlockName;

  /** @suppress {duplicate } */
  function _glGetActiveUniformBlockiv(program, uniformBlockIndex, pname, params) {
      if (!params) {
        // GLES2 specification does not specify how to behave if params is a null pointer. Since calling this function does not make sense
        // if params == null, issue a GL error to notify user about it.
        GL.recordError(0x501 /* GL_INVALID_VALUE */);
        return;
      }
      program = GL.programs[program];
  
      if (pname == 0x8A41 /* GL_UNIFORM_BLOCK_NAME_LENGTH */) {
        var name = GLctx.getActiveUniformBlockName(program, uniformBlockIndex);
        HEAP32[((params)>>2)] = name.length+1;
        return;
      }
  
      var result = GLctx.getActiveUniformBlockParameter(program, uniformBlockIndex, pname);
      if (result === null) return; // If an error occurs, nothing should be written to params.
      if (pname == 0x8A43 /*GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES*/) {
        for (var i = 0; i < result.length; i++) {
          HEAP32[(((params)+(i*4))>>2)] = result[i];
        }
      } else {
        HEAP32[((params)>>2)] = result;
      }
    }
  var _emscripten_glGetActiveUniformBlockiv = _glGetActiveUniformBlockiv;

  /** @suppress {duplicate } */
  function _glGetActiveUniformsiv(program, uniformCount, uniformIndices, pname, params) {
      if (!params) {
        // GLES2 specification does not specify how to behave if params is a null pointer. Since calling this function does not make sense
        // if params == null, issue a GL error to notify user about it.
        GL.recordError(0x501 /* GL_INVALID_VALUE */);
        return;
      }
      if (uniformCount > 0 && uniformIndices == 0) {
        GL.recordError(0x501 /* GL_INVALID_VALUE */);
        return;
      }
      program = GL.programs[program];
      var ids = [];
      for (var i = 0; i < uniformCount; i++) {
        ids.push(HEAP32[(((uniformIndices)+(i*4))>>2)]);
      }
  
      var result = GLctx.getActiveUniforms(program, ids, pname);
      if (!result) return; // GL spec: If an error is generated, nothing is written out to params.
  
      var len = result.length;
      for (var i = 0; i < len; i++) {
        HEAP32[(((params)+(i*4))>>2)] = result[i];
      }
    }
  var _emscripten_glGetActiveUniformsiv = _glGetActiveUniformsiv;

  /** @suppress {duplicate } */
  function _glGetAttachedShaders(program, maxCount, count, shaders) {
      var result = GLctx.getAttachedShaders(GL.programs[program]);
      var len = result.length;
      if (len > maxCount) {
        len = maxCount;
      }
      HEAP32[((count)>>2)] = len;
      for (var i = 0; i < len; ++i) {
        var id = GL.shaders.indexOf(result[i]);
        HEAP32[(((shaders)+(i*4))>>2)] = id;
      }
    }
  var _emscripten_glGetAttachedShaders = _glGetAttachedShaders;

  
  /** @suppress {duplicate } */
  function _glGetAttribLocation(program, name) {
      return GLctx.getAttribLocation(GL.programs[program], UTF8ToString(name));
    }
  var _emscripten_glGetAttribLocation = _glGetAttribLocation;

  function readI53FromI64(ptr) {
      return HEAPU32[ptr>>2] + HEAP32[ptr+4>>2] * 4294967296;
    }
  
  function readI53FromU64(ptr) {
      return HEAPU32[ptr>>2] + HEAPU32[ptr+4>>2] * 4294967296;
    }
  function writeI53ToI64(ptr, num) {
      HEAPU32[ptr>>2] = num;
      HEAPU32[ptr+4>>2] = (num - HEAPU32[ptr>>2])/4294967296;
      var deserialized = (num >= 0) ? readI53FromU64(ptr) : readI53FromI64(ptr);
      if (deserialized != num) warnOnce('writeI53ToI64() out of range: serialized JS Number ' + num + ' to Wasm heap as bytes lo=' + ptrToString(HEAPU32[ptr>>2]) + ', hi=' + ptrToString(HEAPU32[ptr+4>>2]) + ', which deserializes back to ' + deserialized + ' instead!');
    }
  
  function emscriptenWebGLGet(name_, p, type) {
      // Guard against user passing a null pointer.
      // Note that GLES2 spec does not say anything about how passing a null pointer should be treated.
      // Testing on desktop core GL 3, the application crashes on glGetIntegerv to a null pointer, but
      // better to report an error instead of doing anything random.
      if (!p) {
        GL.recordError(0x501 /* GL_INVALID_VALUE */);
        return;
      }
      var ret = undefined;
      switch (name_) { // Handle a few trivial GLES values
        case 0x8DFA: // GL_SHADER_COMPILER
          ret = 1;
          break;
        case 0x8DF8: // GL_SHADER_BINARY_FORMATS
          if (type != 0 && type != 1) {
            GL.recordError(0x500); // GL_INVALID_ENUM
          }
          return; // Do not write anything to the out pointer, since no binary formats are supported.
        case 0x87FE: // GL_NUM_PROGRAM_BINARY_FORMATS
        case 0x8DF9: // GL_NUM_SHADER_BINARY_FORMATS
          ret = 0;
          break;
        case 0x86A2: // GL_NUM_COMPRESSED_TEXTURE_FORMATS
          // WebGL doesn't have GL_NUM_COMPRESSED_TEXTURE_FORMATS (it's obsolete since GL_COMPRESSED_TEXTURE_FORMATS returns a JS array that can be queried for length),
          // so implement it ourselves to allow C++ GLES2 code get the length.
          var formats = GLctx.getParameter(0x86A3 /*GL_COMPRESSED_TEXTURE_FORMATS*/);
          ret = formats ? formats.length : 0;
          break;
  
        case 0x821D: // GL_NUM_EXTENSIONS
          if (GL.currentContext.version < 2) {
            GL.recordError(0x502 /* GL_INVALID_OPERATION */); // Calling GLES3/WebGL2 function with a GLES2/WebGL1 context
            return;
          }
          // .getSupportedExtensions() can return null if context is lost, so coerce to empty array.
          var exts = GLctx.getSupportedExtensions() || [];
          ret = 2 * exts.length; // each extension is duplicated, first in unprefixed WebGL form, and then a second time with "GL_" prefix.
          break;
        case 0x821B: // GL_MAJOR_VERSION
        case 0x821C: // GL_MINOR_VERSION
          if (GL.currentContext.version < 2) {
            GL.recordError(0x500); // GL_INVALID_ENUM
            return;
          }
          ret = name_ == 0x821B ? 3 : 0; // return version 3.0
          break;
      }
  
      if (ret === undefined) {
        var result = GLctx.getParameter(name_);
        switch (typeof result) {
          case "number":
            ret = result;
            break;
          case "boolean":
            ret = result ? 1 : 0;
            break;
          case "string":
            GL.recordError(0x500); // GL_INVALID_ENUM
            return;
          case "object":
            if (result === null) {
              // null is a valid result for some (e.g., which buffer is bound - perhaps nothing is bound), but otherwise
              // can mean an invalid name_, which we need to report as an error
              switch (name_) {
                case 0x8894: // ARRAY_BUFFER_BINDING
                case 0x8B8D: // CURRENT_PROGRAM
                case 0x8895: // ELEMENT_ARRAY_BUFFER_BINDING
                case 0x8CA6: // FRAMEBUFFER_BINDING or DRAW_FRAMEBUFFER_BINDING
                case 0x8CA7: // RENDERBUFFER_BINDING
                case 0x8069: // TEXTURE_BINDING_2D
                case 0x85B5: // WebGL 2 GL_VERTEX_ARRAY_BINDING, or WebGL 1 extension OES_vertex_array_object GL_VERTEX_ARRAY_BINDING_OES
                case 0x8F36: // COPY_READ_BUFFER_BINDING or COPY_READ_BUFFER
                case 0x8F37: // COPY_WRITE_BUFFER_BINDING or COPY_WRITE_BUFFER
                case 0x88ED: // PIXEL_PACK_BUFFER_BINDING
                case 0x88EF: // PIXEL_UNPACK_BUFFER_BINDING
                case 0x8CAA: // READ_FRAMEBUFFER_BINDING
                case 0x8919: // SAMPLER_BINDING
                case 0x8C1D: // TEXTURE_BINDING_2D_ARRAY
                case 0x806A: // TEXTURE_BINDING_3D
                case 0x8E25: // TRANSFORM_FEEDBACK_BINDING
                case 0x8C8F: // TRANSFORM_FEEDBACK_BUFFER_BINDING
                case 0x8A28: // UNIFORM_BUFFER_BINDING
                case 0x8514: { // TEXTURE_BINDING_CUBE_MAP
                  ret = 0;
                  break;
                }
                default: {
                  GL.recordError(0x500); // GL_INVALID_ENUM
                  return;
                }
              }
            } else if (result instanceof Float32Array ||
                       result instanceof Uint32Array ||
                       result instanceof Int32Array ||
                       result instanceof Array) {
              for (var i = 0; i < result.length; ++i) {
                switch (type) {
                  case 0: HEAP32[(((p)+(i*4))>>2)] = result[i]; break;
                  case 2: HEAPF32[(((p)+(i*4))>>2)] = result[i]; break;
                  case 4: HEAP8[(((p)+(i))>>0)] = result[i] ? 1 : 0; break;
                }
              }
              return;
            } else {
              try {
                ret = result.name | 0;
              } catch(e) {
                GL.recordError(0x500); // GL_INVALID_ENUM
                err('GL_INVALID_ENUM in glGet' + type + 'v: Unknown object returned from WebGL getParameter(' + name_ + ')! (error: ' + e + ')');
                return;
              }
            }
            break;
          default:
            GL.recordError(0x500); // GL_INVALID_ENUM
            err('GL_INVALID_ENUM in glGet' + type + 'v: Native code calling glGet' + type + 'v(' + name_ + ') and it returns ' + result + ' of type ' + typeof(result) + '!');
            return;
        }
      }
  
      switch (type) {
        case 1: writeI53ToI64(p, ret); break;
        case 0: HEAP32[((p)>>2)] = ret; break;
        case 2:   HEAPF32[((p)>>2)] = ret; break;
        case 4: HEAP8[((p)>>0)] = ret ? 1 : 0; break;
      }
    }
  
  /** @suppress {duplicate } */
  function _glGetBooleanv(name_, p) {
      emscriptenWebGLGet(name_, p, 4);
    }
  var _emscripten_glGetBooleanv = _glGetBooleanv;

  /** @suppress {duplicate } */
  function _glGetBufferParameteri64v(target, value, data) {
      if (!data) {
        // GLES2 specification does not specify how to behave if data is a null pointer. Since calling this function does not make sense
        // if data == null, issue a GL error to notify user about it.
        GL.recordError(0x501 /* GL_INVALID_VALUE */);
        return;
      }
      writeI53ToI64(data, GLctx.getBufferParameter(target, value));
    }
  var _emscripten_glGetBufferParameteri64v = _glGetBufferParameteri64v;

  /** @suppress {duplicate } */
  function _glGetBufferParameteriv(target, value, data) {
      if (!data) {
        // GLES2 specification does not specify how to behave if data is a null pointer. Since calling this function does not make sense
        // if data == null, issue a GL error to notify user about it.
        GL.recordError(0x501 /* GL_INVALID_VALUE */);
        return;
      }
      HEAP32[((data)>>2)] = GLctx.getBufferParameter(target, value);
    }
  var _emscripten_glGetBufferParameteriv = _glGetBufferParameteriv;

  
  /** @suppress {duplicate } */
  function _glGetBufferPointerv(target, pname, params) {
      if (pname == 0x88BD/*GL_BUFFER_MAP_POINTER*/) {
        var ptr = 0;
        var mappedBuffer = GL.mappedBuffers[emscriptenWebGLGetBufferBinding(target)];
        if (mappedBuffer) {
          ptr = mappedBuffer.mem;
        }
        HEAP32[((params)>>2)] = ptr;
      } else {
        GL.recordError(0x500/*GL_INVALID_ENUM*/);
        err('GL_INVALID_ENUM in glGetBufferPointerv');
      }
    }
  var _emscripten_glGetBufferPointerv = _glGetBufferPointerv;

  /** @suppress {duplicate } */
  function _glGetError() {
      var error = GLctx.getError() || GL.lastError;
      GL.lastError = 0/*GL_NO_ERROR*/;
      return error;
    }
  var _emscripten_glGetError = _glGetError;

  
  /** @suppress {duplicate } */
  function _glGetFloatv(name_, p) {
      emscriptenWebGLGet(name_, p, 2);
    }
  var _emscripten_glGetFloatv = _glGetFloatv;

  /** @suppress {duplicate } */
  function _glGetFragDataLocation(program, name) {
      return GLctx.getFragDataLocation(GL.programs[program], UTF8ToString(name));
    }
  var _emscripten_glGetFragDataLocation = _glGetFragDataLocation;

  /** @suppress {duplicate } */
  function _glGetFramebufferAttachmentParameteriv(target, attachment, pname, params) {
      var result = GLctx.getFramebufferAttachmentParameter(target, attachment, pname);
      if (result instanceof WebGLRenderbuffer ||
          result instanceof WebGLTexture) {
        result = result.name | 0;
      }
      HEAP32[((params)>>2)] = result;
    }
  var _emscripten_glGetFramebufferAttachmentParameteriv = _glGetFramebufferAttachmentParameteriv;

  function emscriptenWebGLGetIndexed(target, index, data, type) {
      if (!data) {
        // GLES2 specification does not specify how to behave if data is a null pointer. Since calling this function does not make sense
        // if data == null, issue a GL error to notify user about it.
        GL.recordError(0x501 /* GL_INVALID_VALUE */);
        return;
      }
      var result = GLctx.getIndexedParameter(target, index);
      var ret;
      switch (typeof result) {
        case 'boolean':
          ret = result ? 1 : 0;
          break;
        case 'number':
          ret = result;
          break;
        case 'object':
          if (result === null) {
            switch (target) {
              case 0x8C8F: // TRANSFORM_FEEDBACK_BUFFER_BINDING
              case 0x8A28: // UNIFORM_BUFFER_BINDING
                ret = 0;
                break;
              default: {
                GL.recordError(0x500); // GL_INVALID_ENUM
                return;
              }
            }
          } else if (result instanceof WebGLBuffer) {
            ret = result.name | 0;
          } else {
            GL.recordError(0x500); // GL_INVALID_ENUM
            return;
          }
          break;
        default:
          GL.recordError(0x500); // GL_INVALID_ENUM
          return;
      }
  
      switch (type) {
        case 1: writeI53ToI64(data, ret); break;
        case 0: HEAP32[((data)>>2)] = ret; break;
        case 2: HEAPF32[((data)>>2)] = ret; break;
        case 4: HEAP8[((data)>>0)] = ret ? 1 : 0; break;
        default: throw 'internal emscriptenWebGLGetIndexed() error, bad type: ' + type;
      }
    }
  /** @suppress {duplicate } */
  function _glGetInteger64i_v(target, index, data) {
      emscriptenWebGLGetIndexed(target, index, data, 1);
    }
  var _emscripten_glGetInteger64i_v = _glGetInteger64i_v;

  /** @suppress {duplicate } */
  function _glGetInteger64v(name_, p) {
      emscriptenWebGLGet(name_, p, 1);
    }
  var _emscripten_glGetInteger64v = _glGetInteger64v;

  /** @suppress {duplicate } */
  function _glGetIntegeri_v(target, index, data) {
      emscriptenWebGLGetIndexed(target, index, data, 0);
    }
  var _emscripten_glGetIntegeri_v = _glGetIntegeri_v;

  
  /** @suppress {duplicate } */
  function _glGetIntegerv(name_, p) {
      emscriptenWebGLGet(name_, p, 0);
    }
  var _emscripten_glGetIntegerv = _glGetIntegerv;

  /** @suppress {duplicate } */
  function _glGetInternalformativ(target, internalformat, pname, bufSize, params) {
      if (bufSize < 0) {
        GL.recordError(0x501 /* GL_INVALID_VALUE */);
        return;
      }
      if (!params) {
        // GLES3 specification does not specify how to behave if values is a null pointer. Since calling this function does not make sense
        // if values == null, issue a GL error to notify user about it.
        GL.recordError(0x501 /* GL_INVALID_VALUE */);
        return;
      }
      var ret = GLctx.getInternalformatParameter(target, internalformat, pname);
      if (ret === null) return;
      for (var i = 0; i < ret.length && i < bufSize; ++i) {
        HEAP32[(((params)+(i*4))>>2)] = ret[i];
      }
    }
  var _emscripten_glGetInternalformativ = _glGetInternalformativ;

  /** @suppress {duplicate } */
  function _glGetProgramBinary(program, bufSize, length, binaryFormat, binary) {
      GL.recordError(0x502/*GL_INVALID_OPERATION*/);
    }
  var _emscripten_glGetProgramBinary = _glGetProgramBinary;

  /** @suppress {duplicate } */
  function _glGetProgramInfoLog(program, maxLength, length, infoLog) {
      var log = GLctx.getProgramInfoLog(GL.programs[program]);
      if (log === null) log = '(unknown error)';
      var numBytesWrittenExclNull = (maxLength > 0 && infoLog) ? stringToUTF8(log, infoLog, maxLength) : 0;
      if (length) HEAP32[((length)>>2)] = numBytesWrittenExclNull;
    }
  var _emscripten_glGetProgramInfoLog = _glGetProgramInfoLog;

  /** @suppress {duplicate } */
  function _glGetProgramiv(program, pname, p) {
      if (!p) {
        // GLES2 specification does not specify how to behave if p is a null pointer. Since calling this function does not make sense
        // if p == null, issue a GL error to notify user about it.
        GL.recordError(0x501 /* GL_INVALID_VALUE */);
        return;
      }
  
      if (program >= GL.counter) {
        GL.recordError(0x501 /* GL_INVALID_VALUE */);
        return;
      }
  
      program = GL.programs[program];
  
      if (pname == 0x8B84) { // GL_INFO_LOG_LENGTH
        var log = GLctx.getProgramInfoLog(program);
        if (log === null) log = '(unknown error)';
        HEAP32[((p)>>2)] = log.length + 1;
      } else if (pname == 0x8B87 /* GL_ACTIVE_UNIFORM_MAX_LENGTH */) {
        if (!program.maxUniformLength) {
          for (var i = 0; i < GLctx.getProgramParameter(program, 0x8B86/*GL_ACTIVE_UNIFORMS*/); ++i) {
            program.maxUniformLength = Math.max(program.maxUniformLength, GLctx.getActiveUniform(program, i).name.length+1);
          }
        }
        HEAP32[((p)>>2)] = program.maxUniformLength;
      } else if (pname == 0x8B8A /* GL_ACTIVE_ATTRIBUTE_MAX_LENGTH */) {
        if (!program.maxAttributeLength) {
          for (var i = 0; i < GLctx.getProgramParameter(program, 0x8B89/*GL_ACTIVE_ATTRIBUTES*/); ++i) {
            program.maxAttributeLength = Math.max(program.maxAttributeLength, GLctx.getActiveAttrib(program, i).name.length+1);
          }
        }
        HEAP32[((p)>>2)] = program.maxAttributeLength;
      } else if (pname == 0x8A35 /* GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH */) {
        if (!program.maxUniformBlockNameLength) {
          for (var i = 0; i < GLctx.getProgramParameter(program, 0x8A36/*GL_ACTIVE_UNIFORM_BLOCKS*/); ++i) {
            program.maxUniformBlockNameLength = Math.max(program.maxUniformBlockNameLength, GLctx.getActiveUniformBlockName(program, i).length+1);
          }
        }
        HEAP32[((p)>>2)] = program.maxUniformBlockNameLength;
      } else {
        HEAP32[((p)>>2)] = GLctx.getProgramParameter(program, pname);
      }
    }
  var _emscripten_glGetProgramiv = _glGetProgramiv;

  
  /** @suppress {duplicate } */
  function _glGetQueryObjecti64vEXT(id, pname, params) {
      if (!params) {
        // GLES2 specification does not specify how to behave if params is a null pointer. Since calling this function does not make sense
        // if p == null, issue a GL error to notify user about it.
        GL.recordError(0x501 /* GL_INVALID_VALUE */);
        return;
      }
      var query = GL.queries[id];
      var param;
      if (GL.currentContext.version < 2)
      {
        param = GLctx.disjointTimerQueryExt['getQueryObjectEXT'](query, pname);
      }
      else {
        param = GLctx.getQueryParameter(query, pname);
      }
      var ret;
      if (typeof param == 'boolean') {
        ret = param ? 1 : 0;
      } else {
        ret = param;
      }
      writeI53ToI64(params, ret);
    }
  var _emscripten_glGetQueryObjecti64vEXT = _glGetQueryObjecti64vEXT;

  /** @suppress {duplicate } */
  function _glGetQueryObjectivEXT(id, pname, params) {
      if (!params) {
        // GLES2 specification does not specify how to behave if params is a null pointer. Since calling this function does not make sense
        // if p == null, issue a GL error to notify user about it.
        GL.recordError(0x501 /* GL_INVALID_VALUE */);
        return;
      }
      var query = GL.queries[id];
      var param = GLctx.disjointTimerQueryExt['getQueryObjectEXT'](query, pname);
      var ret;
      if (typeof param == 'boolean') {
        ret = param ? 1 : 0;
      } else {
        ret = param;
      }
      HEAP32[((params)>>2)] = ret;
    }
  var _emscripten_glGetQueryObjectivEXT = _glGetQueryObjectivEXT;

  
  /** @suppress {duplicate } */
  var _glGetQueryObjectui64vEXT = _glGetQueryObjecti64vEXT;
  var _emscripten_glGetQueryObjectui64vEXT = _glGetQueryObjectui64vEXT;

  /** @suppress {duplicate } */
  function _glGetQueryObjectuiv(id, pname, params) {
      if (!params) {
        // GLES2 specification does not specify how to behave if params is a null pointer. Since calling this function does not make sense
        // if p == null, issue a GL error to notify user about it.
        GL.recordError(0x501 /* GL_INVALID_VALUE */);
        return;
      }
      var query = GL.queries[id];
      var param = GLctx.getQueryParameter(query, pname);
      var ret;
      if (typeof param == 'boolean') {
        ret = param ? 1 : 0;
      } else {
        ret = param;
      }
      HEAP32[((params)>>2)] = ret;
    }
  var _emscripten_glGetQueryObjectuiv = _glGetQueryObjectuiv;

  
  /** @suppress {duplicate } */
  var _glGetQueryObjectuivEXT = _glGetQueryObjectivEXT;
  var _emscripten_glGetQueryObjectuivEXT = _glGetQueryObjectuivEXT;

  /** @suppress {duplicate } */
  function _glGetQueryiv(target, pname, params) {
      if (!params) {
        // GLES2 specification does not specify how to behave if params is a null pointer. Since calling this function does not make sense
        // if p == null, issue a GL error to notify user about it.
        GL.recordError(0x501 /* GL_INVALID_VALUE */);
        return;
      }
      HEAP32[((params)>>2)] = GLctx.getQuery(target, pname);
    }
  var _emscripten_glGetQueryiv = _glGetQueryiv;

  /** @suppress {duplicate } */
  function _glGetQueryivEXT(target, pname, params) {
      if (!params) {
        // GLES2 specification does not specify how to behave if params is a null pointer. Since calling this function does not make sense
        // if p == null, issue a GL error to notify user about it.
        GL.recordError(0x501 /* GL_INVALID_VALUE */);
        return;
      }
      HEAP32[((params)>>2)] = GLctx.disjointTimerQueryExt['getQueryEXT'](target, pname);
    }
  var _emscripten_glGetQueryivEXT = _glGetQueryivEXT;

  /** @suppress {duplicate } */
  function _glGetRenderbufferParameteriv(target, pname, params) {
      if (!params) {
        // GLES2 specification does not specify how to behave if params is a null pointer. Since calling this function does not make sense
        // if params == null, issue a GL error to notify user about it.
        GL.recordError(0x501 /* GL_INVALID_VALUE */);
        return;
      }
      HEAP32[((params)>>2)] = GLctx.getRenderbufferParameter(target, pname);
    }
  var _emscripten_glGetRenderbufferParameteriv = _glGetRenderbufferParameteriv;

  /** @suppress {duplicate } */
  function _glGetSamplerParameterfv(sampler, pname, params) {
      if (!params) {
        // GLES3 specification does not specify how to behave if params is a null pointer. Since calling this function does not make sense
        // if p == null, issue a GL error to notify user about it.
        GL.recordError(0x501 /* GL_INVALID_VALUE */);
        return;
      }
      HEAPF32[((params)>>2)] = GLctx.getSamplerParameter(GL.samplers[sampler], pname);
    }
  var _emscripten_glGetSamplerParameterfv = _glGetSamplerParameterfv;

  /** @suppress {duplicate } */
  function _glGetSamplerParameteriv(sampler, pname, params) {
      if (!params) {
        // GLES3 specification does not specify how to behave if params is a null pointer. Since calling this function does not make sense
        // if p == null, issue a GL error to notify user about it.
        GL.recordError(0x501 /* GL_INVALID_VALUE */);
        return;
      }
      HEAP32[((params)>>2)] = GLctx.getSamplerParameter(GL.samplers[sampler], pname);
    }
  var _emscripten_glGetSamplerParameteriv = _glGetSamplerParameteriv;

  
  /** @suppress {duplicate } */
  function _glGetShaderInfoLog(shader, maxLength, length, infoLog) {
      var log = GLctx.getShaderInfoLog(GL.shaders[shader]);
      if (log === null) log = '(unknown error)';
      var numBytesWrittenExclNull = (maxLength > 0 && infoLog) ? stringToUTF8(log, infoLog, maxLength) : 0;
      if (length) HEAP32[((length)>>2)] = numBytesWrittenExclNull;
    }
  var _emscripten_glGetShaderInfoLog = _glGetShaderInfoLog;

  /** @suppress {duplicate } */
  function _glGetShaderPrecisionFormat(shaderType, precisionType, range, precision) {
      var result = GLctx.getShaderPrecisionFormat(shaderType, precisionType);
      HEAP32[((range)>>2)] = result.rangeMin;
      HEAP32[(((range)+(4))>>2)] = result.rangeMax;
      HEAP32[((precision)>>2)] = result.precision;
    }
  var _emscripten_glGetShaderPrecisionFormat = _glGetShaderPrecisionFormat;

  /** @suppress {duplicate } */
  function _glGetShaderSource(shader, bufSize, length, source) {
      var result = GLctx.getShaderSource(GL.shaders[shader]);
      if (!result) return; // If an error occurs, nothing will be written to length or source.
      var numBytesWrittenExclNull = (bufSize > 0 && source) ? stringToUTF8(result, source, bufSize) : 0;
      if (length) HEAP32[((length)>>2)] = numBytesWrittenExclNull;
    }
  var _emscripten_glGetShaderSource = _glGetShaderSource;

  /** @suppress {duplicate } */
  function _glGetShaderiv(shader, pname, p) {
      if (!p) {
        // GLES2 specification does not specify how to behave if p is a null pointer. Since calling this function does not make sense
        // if p == null, issue a GL error to notify user about it.
        GL.recordError(0x501 /* GL_INVALID_VALUE */);
        return;
      }
      if (pname == 0x8B84) { // GL_INFO_LOG_LENGTH
        var log = GLctx.getShaderInfoLog(GL.shaders[shader]);
        if (log === null) log = '(unknown error)';
        // The GLES2 specification says that if the shader has an empty info log,
        // a value of 0 is returned. Otherwise the log has a null char appended.
        // (An empty string is falsey, so we can just check that instead of
        // looking at log.length.)
        var logLength = log ? log.length + 1 : 0;
        HEAP32[((p)>>2)] = logLength;
      } else if (pname == 0x8B88) { // GL_SHADER_SOURCE_LENGTH
        var source = GLctx.getShaderSource(GL.shaders[shader]);
        // source may be a null, or the empty string, both of which are falsey
        // values that we report a 0 length for.
        var sourceLength = source ? source.length + 1 : 0;
        HEAP32[((p)>>2)] = sourceLength;
      } else {
        HEAP32[((p)>>2)] = GLctx.getShaderParameter(GL.shaders[shader], pname);
      }
    }
  var _emscripten_glGetShaderiv = _glGetShaderiv;

  
  /** @suppress {duplicate } */
  function _glGetString(name_) {
      var ret = GL.stringCache[name_];
      if (!ret) {
        switch (name_) {
          case 0x1F03 /* GL_EXTENSIONS */:
            var exts = GLctx.getSupportedExtensions() || []; // .getSupportedExtensions() can return null if context is lost, so coerce to empty array.
            exts = exts.concat(exts.map(function(e) { return "GL_" + e; }));
            ret = stringToNewUTF8(exts.join(' '));
            break;
          case 0x1F00 /* GL_VENDOR */:
          case 0x1F01 /* GL_RENDERER */:
          case 0x9245 /* UNMASKED_VENDOR_WEBGL */:
          case 0x9246 /* UNMASKED_RENDERER_WEBGL */:
            var s = GLctx.getParameter(name_);
            if (!s) {
              GL.recordError(0x500/*GL_INVALID_ENUM*/);
            }
            ret = s && stringToNewUTF8(s);
            break;
  
          case 0x1F02 /* GL_VERSION */:
            var glVersion = GLctx.getParameter(0x1F02 /*GL_VERSION*/);
            // return GLES version string corresponding to the version of the WebGL context
            if (GL.currentContext.version >= 2) glVersion = 'OpenGL ES 3.0 (' + glVersion + ')';
            else
            {
              glVersion = 'OpenGL ES 2.0 (' + glVersion + ')';
            }
            ret = stringToNewUTF8(glVersion);
            break;
          case 0x8B8C /* GL_SHADING_LANGUAGE_VERSION */:
            var glslVersion = GLctx.getParameter(0x8B8C /*GL_SHADING_LANGUAGE_VERSION*/);
            // extract the version number 'N.M' from the string 'WebGL GLSL ES N.M ...'
            var ver_re = /^WebGL GLSL ES ([0-9]\.[0-9][0-9]?)(?:$| .*)/;
            var ver_num = glslVersion.match(ver_re);
            if (ver_num !== null) {
              if (ver_num[1].length == 3) ver_num[1] = ver_num[1] + '0'; // ensure minor version has 2 digits
              glslVersion = 'OpenGL ES GLSL ES ' + ver_num[1] + ' (' + glslVersion + ')';
            }
            ret = stringToNewUTF8(glslVersion);
            break;
          default:
            GL.recordError(0x500/*GL_INVALID_ENUM*/);
            // fall through
        }
        GL.stringCache[name_] = ret;
      }
      return ret;
    }
  var _emscripten_glGetString = _glGetString;

  /** @suppress {duplicate } */
  function _glGetStringi(name, index) {
      if (GL.currentContext.version < 2) {
        GL.recordError(0x502 /* GL_INVALID_OPERATION */); // Calling GLES3/WebGL2 function with a GLES2/WebGL1 context
        return 0;
      }
      var stringiCache = GL.stringiCache[name];
      if (stringiCache) {
        if (index < 0 || index >= stringiCache.length) {
          GL.recordError(0x501/*GL_INVALID_VALUE*/);
          return 0;
        }
        return stringiCache[index];
      }
      switch (name) {
        case 0x1F03 /* GL_EXTENSIONS */:
          var exts = GLctx.getSupportedExtensions() || []; // .getSupportedExtensions() can return null if context is lost, so coerce to empty array.
          exts = exts.concat(exts.map(function(e) { return "GL_" + e; }));
          exts = exts.map(function(e) { return stringToNewUTF8(e); });
  
          stringiCache = GL.stringiCache[name] = exts;
          if (index < 0 || index >= stringiCache.length) {
            GL.recordError(0x501/*GL_INVALID_VALUE*/);
            return 0;
          }
          return stringiCache[index];
        default:
          GL.recordError(0x500/*GL_INVALID_ENUM*/);
          return 0;
      }
    }
  var _emscripten_glGetStringi = _glGetStringi;

  /** @suppress {duplicate } */
  function _glGetSynciv(sync, pname, bufSize, length, values) {
      if (bufSize < 0) {
        // GLES3 specification does not specify how to behave if bufSize < 0, however in the spec wording for glGetInternalformativ, it does say that GL_INVALID_VALUE should be raised,
        // so raise GL_INVALID_VALUE here as well.
        GL.recordError(0x501 /* GL_INVALID_VALUE */);
        return;
      }
      if (!values) {
        // GLES3 specification does not specify how to behave if values is a null pointer. Since calling this function does not make sense
        // if values == null, issue a GL error to notify user about it.
        GL.recordError(0x501 /* GL_INVALID_VALUE */);
        return;
      }
      var ret = GLctx.getSyncParameter(GL.syncs[sync], pname);
      if (ret !== null) {
        HEAP32[((values)>>2)] = ret;
        if (length) HEAP32[((length)>>2)] = 1; // Report a single value outputted.
      }
    }
  var _emscripten_glGetSynciv = _glGetSynciv;

  /** @suppress {duplicate } */
  function _glGetTexParameterfv(target, pname, params) {
      if (!params) {
        // GLES2 specification does not specify how to behave if params is a null pointer. Since calling this function does not make sense
        // if p == null, issue a GL error to notify user about it.
        GL.recordError(0x501 /* GL_INVALID_VALUE */);
        return;
      }
      HEAPF32[((params)>>2)] = GLctx.getTexParameter(target, pname);
    }
  var _emscripten_glGetTexParameterfv = _glGetTexParameterfv;

  /** @suppress {duplicate } */
  function _glGetTexParameteriv(target, pname, params) {
      if (!params) {
        // GLES2 specification does not specify how to behave if params is a null pointer. Since calling this function does not make sense
        // if p == null, issue a GL error to notify user about it.
        GL.recordError(0x501 /* GL_INVALID_VALUE */);
        return;
      }
      HEAP32[((params)>>2)] = GLctx.getTexParameter(target, pname);
    }
  var _emscripten_glGetTexParameteriv = _glGetTexParameteriv;

  /** @suppress {duplicate } */
  function _glGetTransformFeedbackVarying(program, index, bufSize, length, size, type, name) {
      program = GL.programs[program];
      var info = GLctx.getTransformFeedbackVarying(program, index);
      if (!info) return; // If an error occurred, the return parameters length, size, type and name will be unmodified.
  
      if (name && bufSize > 0) {
        var numBytesWrittenExclNull = stringToUTF8(info.name, name, bufSize);
        if (length) HEAP32[((length)>>2)] = numBytesWrittenExclNull;
      } else {
        if (length) HEAP32[((length)>>2)] = 0;
      }
  
      if (size) HEAP32[((size)>>2)] = info.size;
      if (type) HEAP32[((type)>>2)] = info.type;
    }
  var _emscripten_glGetTransformFeedbackVarying = _glGetTransformFeedbackVarying;

  /** @suppress {duplicate } */
  function _glGetUniformBlockIndex(program, uniformBlockName) {
      return GLctx.getUniformBlockIndex(GL.programs[program], UTF8ToString(uniformBlockName));
    }
  var _emscripten_glGetUniformBlockIndex = _glGetUniformBlockIndex;

  /** @suppress {duplicate } */
  function _glGetUniformIndices(program, uniformCount, uniformNames, uniformIndices) {
      if (!uniformIndices) {
        // GLES2 specification does not specify how to behave if uniformIndices is a null pointer. Since calling this function does not make sense
        // if uniformIndices == null, issue a GL error to notify user about it.
        GL.recordError(0x501 /* GL_INVALID_VALUE */);
        return;
      }
      if (uniformCount > 0 && (uniformNames == 0 || uniformIndices == 0)) {
        GL.recordError(0x501 /* GL_INVALID_VALUE */);
        return;
      }
      program = GL.programs[program];
      var names = [];
      for (var i = 0; i < uniformCount; i++)
        names.push(UTF8ToString(HEAP32[(((uniformNames)+(i*4))>>2)]));
  
      var result = GLctx.getUniformIndices(program, names);
      if (!result) return; // GL spec: If an error is generated, nothing is written out to uniformIndices.
  
      var len = result.length;
      for (var i = 0; i < len; i++) {
        HEAP32[(((uniformIndices)+(i*4))>>2)] = result[i];
      }
    }
  var _emscripten_glGetUniformIndices = _glGetUniformIndices;

  /** @suppress {checkTypes} */
  var jstoi_q = (str) => parseInt(str);
  
  /** @noinline */
  function webglGetLeftBracePos(name) {
      return name.slice(-1) == ']' && name.lastIndexOf('[');
    }
  
  function webglPrepareUniformLocationsBeforeFirstUse(program) {
      var uniformLocsById = program.uniformLocsById, // Maps GLuint -> WebGLUniformLocation
        uniformSizeAndIdsByName = program.uniformSizeAndIdsByName, // Maps name -> [uniform array length, GLuint]
        i, j;
  
      // On the first time invocation of glGetUniformLocation on this shader program:
      // initialize cache data structures and discover which uniforms are arrays.
      if (!uniformLocsById) {
        // maps GLint integer locations to WebGLUniformLocations
        program.uniformLocsById = uniformLocsById = {};
        // maps integer locations back to uniform name strings, so that we can lazily fetch uniform array locations
        program.uniformArrayNamesById = {};
  
        for (i = 0; i < GLctx.getProgramParameter(program, 0x8B86/*GL_ACTIVE_UNIFORMS*/); ++i) {
          var u = GLctx.getActiveUniform(program, i);
          var nm = u.name;
          var sz = u.size;
          var lb = webglGetLeftBracePos(nm);
          var arrayName = lb > 0 ? nm.slice(0, lb) : nm;
  
          // Assign a new location.
          var id = program.uniformIdCounter;
          program.uniformIdCounter += sz;
          // Eagerly get the location of the uniformArray[0] base element.
          // The remaining indices >0 will be left for lazy evaluation to
          // improve performance. Those may never be needed to fetch, if the
          // application fills arrays always in full starting from the first
          // element of the array.
          uniformSizeAndIdsByName[arrayName] = [sz, id];
  
          // Store placeholder integers in place that highlight that these
          // >0 index locations are array indices pending population.
          for(j = 0; j < sz; ++j) {
            uniformLocsById[id] = j;
            program.uniformArrayNamesById[id++] = arrayName;
          }
        }
      }
    }
  
  
  
  /** @suppress {duplicate } */
  function _glGetUniformLocation(program, name) {
  
      name = UTF8ToString(name);
  
      if (program = GL.programs[program]) {
        webglPrepareUniformLocationsBeforeFirstUse(program);
        var uniformLocsById = program.uniformLocsById; // Maps GLuint -> WebGLUniformLocation
        var arrayIndex = 0;
        var uniformBaseName = name;
  
        // Invariant: when populating integer IDs for uniform locations, we must maintain the precondition that
        // arrays reside in contiguous addresses, i.e. for a 'vec4 colors[10];', colors[4] must be at location colors[0]+4.
        // However, user might call glGetUniformLocation(program, "colors") for an array, so we cannot discover based on the user
        // input arguments whether the uniform we are dealing with is an array. The only way to discover which uniforms are arrays
        // is to enumerate over all the active uniforms in the program.
        var leftBrace = webglGetLeftBracePos(name);
  
        // If user passed an array accessor "[index]", parse the array index off the accessor.
        if (leftBrace > 0) {
          arrayIndex = jstoi_q(name.slice(leftBrace + 1)) >>> 0; // "index]", coerce parseInt(']') with >>>0 to treat "foo[]" as "foo[0]" and foo[-1] as unsigned out-of-bounds.
          uniformBaseName = name.slice(0, leftBrace);
        }
  
        // Have we cached the location of this uniform before?
        var sizeAndId = program.uniformSizeAndIdsByName[uniformBaseName]; // A pair [array length, GLint of the uniform location]
  
        // If an uniform with this name exists, and if its index is within the array limits (if it's even an array),
        // query the WebGLlocation, or return an existing cached location.
        if (sizeAndId && arrayIndex < sizeAndId[0]) {
          arrayIndex += sizeAndId[1]; // Add the base location of the uniform to the array index offset.
          if ((uniformLocsById[arrayIndex] = uniformLocsById[arrayIndex] || GLctx.getUniformLocation(program, name))) {
            return arrayIndex;
          }
        }
      }
      else {
        // N.b. we are currently unable to distinguish between GL program IDs that never existed vs GL program IDs that have been deleted,
        // so report GL_INVALID_VALUE in both cases.
        GL.recordError(0x501 /* GL_INVALID_VALUE */);
      }
      return -1;
    }
  var _emscripten_glGetUniformLocation = _glGetUniformLocation;

  function webglGetUniformLocation(location) {
      var p = GLctx.currentProgram;
  
      if (p) {
        var webglLoc = p.uniformLocsById[location];
        // p.uniformLocsById[location] stores either an integer, or a WebGLUniformLocation.
  
        // If an integer, we have not yet bound the location, so do it now. The integer value specifies the array index
        // we should bind to.
        if (typeof webglLoc == 'number') {
          p.uniformLocsById[location] = webglLoc = GLctx.getUniformLocation(p, p.uniformArrayNamesById[location] + (webglLoc > 0 ? '[' + webglLoc + ']' : ''));
        }
        // Else an already cached WebGLUniformLocation, return it.
        return webglLoc;
      } else {
        GL.recordError(0x502/*GL_INVALID_OPERATION*/);
      }
    }
  
  
  /** @suppress{checkTypes} */
  function emscriptenWebGLGetUniform(program, location, params, type) {
      if (!params) {
        // GLES2 specification does not specify how to behave if params is a null pointer. Since calling this function does not make sense
        // if params == null, issue a GL error to notify user about it.
        GL.recordError(0x501 /* GL_INVALID_VALUE */);
        return;
      }
      program = GL.programs[program];
      webglPrepareUniformLocationsBeforeFirstUse(program);
      var data = GLctx.getUniform(program, webglGetUniformLocation(location));
      if (typeof data == 'number' || typeof data == 'boolean') {
        switch (type) {
          case 0: HEAP32[((params)>>2)] = data; break;
          case 2: HEAPF32[((params)>>2)] = data; break;
        }
      } else {
        for (var i = 0; i < data.length; i++) {
          switch (type) {
            case 0: HEAP32[(((params)+(i*4))>>2)] = data[i]; break;
            case 2: HEAPF32[(((params)+(i*4))>>2)] = data[i]; break;
          }
        }
      }
    }
  
  /** @suppress {duplicate } */
  function _glGetUniformfv(program, location, params) {
      emscriptenWebGLGetUniform(program, location, params, 2);
    }
  var _emscripten_glGetUniformfv = _glGetUniformfv;

  
  /** @suppress {duplicate } */
  function _glGetUniformiv(program, location, params) {
      emscriptenWebGLGetUniform(program, location, params, 0);
    }
  var _emscripten_glGetUniformiv = _glGetUniformiv;

  /** @suppress {duplicate } */
  function _glGetUniformuiv(program, location, params) {
      emscriptenWebGLGetUniform(program, location, params, 0);
    }
  var _emscripten_glGetUniformuiv = _glGetUniformuiv;

  /** @suppress{checkTypes} */
  function emscriptenWebGLGetVertexAttrib(index, pname, params, type) {
      if (!params) {
        // GLES2 specification does not specify how to behave if params is a null pointer. Since calling this function does not make sense
        // if params == null, issue a GL error to notify user about it.
        GL.recordError(0x501 /* GL_INVALID_VALUE */);
        return;
      }
      if (GL.currentContext.clientBuffers[index].enabled) {
        err("glGetVertexAttrib*v on client-side array: not supported, bad data returned");
      }
      var data = GLctx.getVertexAttrib(index, pname);
      if (pname == 0x889F/*VERTEX_ATTRIB_ARRAY_BUFFER_BINDING*/) {
        HEAP32[((params)>>2)] = data && data["name"];
      } else if (typeof data == 'number' || typeof data == 'boolean') {
        switch (type) {
          case 0: HEAP32[((params)>>2)] = data; break;
          case 2: HEAPF32[((params)>>2)] = data; break;
          case 5: HEAP32[((params)>>2)] = Math.fround(data); break;
        }
      } else {
        for (var i = 0; i < data.length; i++) {
          switch (type) {
            case 0: HEAP32[(((params)+(i*4))>>2)] = data[i]; break;
            case 2: HEAPF32[(((params)+(i*4))>>2)] = data[i]; break;
            case 5: HEAP32[(((params)+(i*4))>>2)] = Math.fround(data[i]); break;
          }
        }
      }
    }
  /** @suppress {duplicate } */
  function _glGetVertexAttribIiv(index, pname, params) {
      // N.B. This function may only be called if the vertex attribute was specified using the function glVertexAttribI4iv(),
      // otherwise the results are undefined. (GLES3 spec 6.1.12)
      emscriptenWebGLGetVertexAttrib(index, pname, params, 0);
    }
  var _emscripten_glGetVertexAttribIiv = _glGetVertexAttribIiv;

  
  /** @suppress {duplicate } */
  var _glGetVertexAttribIuiv = _glGetVertexAttribIiv;
  var _emscripten_glGetVertexAttribIuiv = _glGetVertexAttribIuiv;

  /** @suppress {duplicate } */
  function _glGetVertexAttribPointerv(index, pname, pointer) {
      if (!pointer) {
        // GLES2 specification does not specify how to behave if pointer is a null pointer. Since calling this function does not make sense
        // if pointer == null, issue a GL error to notify user about it.
        GL.recordError(0x501 /* GL_INVALID_VALUE */);
        return;
      }
      if (GL.currentContext.clientBuffers[index].enabled) {
        err("glGetVertexAttribPointer on client-side array: not supported, bad data returned");
      }
      HEAP32[((pointer)>>2)] = GLctx.getVertexAttribOffset(index, pname);
    }
  var _emscripten_glGetVertexAttribPointerv = _glGetVertexAttribPointerv;

  
  /** @suppress {duplicate } */
  function _glGetVertexAttribfv(index, pname, params) {
      // N.B. This function may only be called if the vertex attribute was specified using the function glVertexAttrib*f(),
      // otherwise the results are undefined. (GLES3 spec 6.1.12)
      emscriptenWebGLGetVertexAttrib(index, pname, params, 2);
    }
  var _emscripten_glGetVertexAttribfv = _glGetVertexAttribfv;

  
  /** @suppress {duplicate } */
  function _glGetVertexAttribiv(index, pname, params) {
      // N.B. This function may only be called if the vertex attribute was specified using the function glVertexAttrib*f(),
      // otherwise the results are undefined. (GLES3 spec 6.1.12)
      emscriptenWebGLGetVertexAttrib(index, pname, params, 5);
    }
  var _emscripten_glGetVertexAttribiv = _glGetVertexAttribiv;

  /** @suppress {duplicate } */
  function _glHint(x0, x1) { GLctx.hint(x0, x1) }
  var _emscripten_glHint = _glHint;

  /** @suppress {duplicate } */
  function _glInvalidateFramebuffer(target, numAttachments, attachments) {
      var list = tempFixedLengthArray[numAttachments];
      for (var i = 0; i < numAttachments; i++) {
        list[i] = HEAP32[(((attachments)+(i*4))>>2)];
      }
  
      GLctx.invalidateFramebuffer(target, list);
    }
  var _emscripten_glInvalidateFramebuffer = _glInvalidateFramebuffer;

  /** @suppress {duplicate } */
  function _glInvalidateSubFramebuffer(target, numAttachments, attachments, x, y, width, height) {
      var list = tempFixedLengthArray[numAttachments];
      for (var i = 0; i < numAttachments; i++) {
        list[i] = HEAP32[(((attachments)+(i*4))>>2)];
      }
  
      GLctx.invalidateSubFramebuffer(target, list, x, y, width, height);
    }
  var _emscripten_glInvalidateSubFramebuffer = _glInvalidateSubFramebuffer;

  /** @suppress {duplicate } */
  function _glIsBuffer(buffer) {
      var b = GL.buffers[buffer];
      if (!b) return 0;
      return GLctx.isBuffer(b);
    }
  var _emscripten_glIsBuffer = _glIsBuffer;

  /** @suppress {duplicate } */
  function _glIsEnabled(x0) { return GLctx.isEnabled(x0) }
  var _emscripten_glIsEnabled = _glIsEnabled;

  /** @suppress {duplicate } */
  function _glIsFramebuffer(framebuffer) {
      var fb = GL.framebuffers[framebuffer];
      if (!fb) return 0;
      return GLctx.isFramebuffer(fb);
    }
  var _emscripten_glIsFramebuffer = _glIsFramebuffer;

  /** @suppress {duplicate } */
  function _glIsProgram(program) {
      program = GL.programs[program];
      if (!program) return 0;
      return GLctx.isProgram(program);
    }
  var _emscripten_glIsProgram = _glIsProgram;

  /** @suppress {duplicate } */
  function _glIsQuery(id) {
      var query = GL.queries[id];
      if (!query) return 0;
      return GLctx.isQuery(query);
    }
  var _emscripten_glIsQuery = _glIsQuery;

  /** @suppress {duplicate } */
  function _glIsQueryEXT(id) {
      var query = GL.queries[id];
      if (!query) return 0;
      return GLctx.disjointTimerQueryExt['isQueryEXT'](query);
    }
  var _emscripten_glIsQueryEXT = _glIsQueryEXT;

  /** @suppress {duplicate } */
  function _glIsRenderbuffer(renderbuffer) {
      var rb = GL.renderbuffers[renderbuffer];
      if (!rb) return 0;
      return GLctx.isRenderbuffer(rb);
    }
  var _emscripten_glIsRenderbuffer = _glIsRenderbuffer;

  /** @suppress {duplicate } */
  function _glIsSampler(id) {
      var sampler = GL.samplers[id];
      if (!sampler) return 0;
      return GLctx.isSampler(sampler);
    }
  var _emscripten_glIsSampler = _glIsSampler;

  /** @suppress {duplicate } */
  function _glIsShader(shader) {
      var s = GL.shaders[shader];
      if (!s) return 0;
      return GLctx.isShader(s);
    }
  var _emscripten_glIsShader = _glIsShader;

  /** @suppress {duplicate } */
  function _glIsSync(sync) {
      return GLctx.isSync(GL.syncs[sync]);
    }
  var _emscripten_glIsSync = _glIsSync;

  /** @suppress {duplicate } */
  function _glIsTexture(id) {
      var texture = GL.textures[id];
      if (!texture) return 0;
      return GLctx.isTexture(texture);
    }
  var _emscripten_glIsTexture = _glIsTexture;

  /** @suppress {duplicate } */
  function _glIsTransformFeedback(id) {
      return GLctx.isTransformFeedback(GL.transformFeedbacks[id]);
    }
  var _emscripten_glIsTransformFeedback = _glIsTransformFeedback;

  /** @suppress {duplicate } */
  function _glIsVertexArray(array) {
  
      var vao = GL.vaos[array];
      if (!vao) return 0;
      return GLctx.isVertexArray(vao);
    }
  var _emscripten_glIsVertexArray = _glIsVertexArray;

  
  /** @suppress {duplicate } */
  var _glIsVertexArrayOES = _glIsVertexArray;
  var _emscripten_glIsVertexArrayOES = _glIsVertexArrayOES;

  /** @suppress {duplicate } */
  function _glLineWidth(x0) { GLctx.lineWidth(x0) }
  var _emscripten_glLineWidth = _glLineWidth;

  /** @suppress {duplicate } */
  function _glLinkProgram(program) {
      program = GL.programs[program];
      GLctx.linkProgram(program);
      // Invalidate earlier computed uniform->ID mappings, those have now become stale
      program.uniformLocsById = 0; // Mark as null-like so that glGetUniformLocation() knows to populate this again.
      program.uniformSizeAndIdsByName = {};
  
    }
  var _emscripten_glLinkProgram = _glLinkProgram;

  
  
  
  /** @suppress {duplicate } */
  function _glMapBufferRange(target, offset, length, access) {
      if ((access & (0x1/*GL_MAP_READ_BIT*/ | 0x20/*GL_MAP_UNSYNCHRONIZED_BIT*/)) != 0) {
        err("glMapBufferRange access does not support MAP_READ or MAP_UNSYNCHRONIZED");
        return 0;
      }
  
      if ((access & 0x2/*GL_MAP_WRITE_BIT*/) == 0) {
        err("glMapBufferRange access must include MAP_WRITE");
        return 0;
      }
  
      if ((access & (0x4/*GL_MAP_INVALIDATE_BUFFER_BIT*/ | 0x8/*GL_MAP_INVALIDATE_RANGE_BIT*/)) == 0) {
        err("glMapBufferRange access must include INVALIDATE_BUFFER or INVALIDATE_RANGE");
        return 0;
      }
  
      if (!emscriptenWebGLValidateMapBufferTarget(target)) {
        GL.recordError(0x500/*GL_INVALID_ENUM*/);
        err('GL_INVALID_ENUM in glMapBufferRange');
        return 0;
      }
  
      var mem = _malloc(length), binding = emscriptenWebGLGetBufferBinding(target);
      if (!mem) return 0;
  
      if (!GL.mappedBuffers[binding]) GL.mappedBuffers[binding] = {};
      binding = GL.mappedBuffers[binding];
      binding.offset = offset;
      binding.length = length;
      binding.mem = mem;
      binding.access = access;
      return mem;
    }
  var _emscripten_glMapBufferRange = _glMapBufferRange;

  /** @suppress {duplicate } */
  function _glPauseTransformFeedback() { GLctx.pauseTransformFeedback() }
  var _emscripten_glPauseTransformFeedback = _glPauseTransformFeedback;

  /** @suppress {duplicate } */
  function _glPixelStorei(pname, param) {
      if (pname == 0xCF5 /* GL_UNPACK_ALIGNMENT */) {
        GL.unpackAlignment = param;
      }
      GLctx.pixelStorei(pname, param);
    }
  var _emscripten_glPixelStorei = _glPixelStorei;

  /** @suppress {duplicate } */
  function _glPolygonOffset(x0, x1) { GLctx.polygonOffset(x0, x1) }
  var _emscripten_glPolygonOffset = _glPolygonOffset;

  /** @suppress {duplicate } */
  function _glProgramBinary(program, binaryFormat, binary, length) {
      GL.recordError(0x500/*GL_INVALID_ENUM*/);
    }
  var _emscripten_glProgramBinary = _glProgramBinary;

  /** @suppress {duplicate } */
  function _glProgramParameteri(program, pname, value) {
      GL.recordError(0x500/*GL_INVALID_ENUM*/);
    }
  var _emscripten_glProgramParameteri = _glProgramParameteri;

  /** @suppress {duplicate } */
  function _glQueryCounterEXT(id, target) {
      GLctx.disjointTimerQueryExt['queryCounterEXT'](GL.queries[id], target);
    }
  var _emscripten_glQueryCounterEXT = _glQueryCounterEXT;

  /** @suppress {duplicate } */
  function _glReadBuffer(x0) { GLctx.readBuffer(x0) }
  var _emscripten_glReadBuffer = _glReadBuffer;

  function computeUnpackAlignedImageSize(width, height, sizePerPixel, alignment) {
      function roundedToNextMultipleOf(x, y) {
        return (x + y - 1) & -y;
      }
      var plainRowSize = width * sizePerPixel;
      var alignedRowSize = roundedToNextMultipleOf(plainRowSize, alignment);
      return height * alignedRowSize;
    }
  
  function colorChannelsInGlTextureFormat(format) {
      // Micro-optimizations for size: map format to size by subtracting smallest enum value (0x1902) from all values first.
      // Also omit the most common size value (1) from the list, which is assumed by formats not on the list.
      var colorChannels = {
        // 0x1902 /* GL_DEPTH_COMPONENT */ - 0x1902: 1,
        // 0x1906 /* GL_ALPHA */ - 0x1902: 1,
        5: 3,
        6: 4,
        // 0x1909 /* GL_LUMINANCE */ - 0x1902: 1,
        8: 2,
        29502: 3,
        29504: 4,
        // 0x1903 /* GL_RED */ - 0x1902: 1,
        26917: 2,
        26918: 2,
        // 0x8D94 /* GL_RED_INTEGER */ - 0x1902: 1,
        29846: 3,
        29847: 4
      };
      return colorChannels[format - 0x1902]||1;
    }
  
  function heapObjectForWebGLType(type) {
      // Micro-optimization for size: Subtract lowest GL enum number (0x1400/* GL_BYTE */) from type to compare
      // smaller values for the heap, for shorter generated code size.
      // Also the type HEAPU16 is not tested for explicitly, but any unrecognized type will return out HEAPU16.
      // (since most types are HEAPU16)
      type -= 0x1400;
      if (type == 0) return HEAP8;
  
      if (type == 1) return HEAPU8;
  
      if (type == 2) return HEAP16;
  
      if (type == 4) return HEAP32;
  
      if (type == 6) return HEAPF32;
  
      if (type == 5
        || type == 28922
        || type == 28520
        || type == 30779
        || type == 30782
        )
        return HEAPU32;
  
      return HEAPU16;
    }
  
  function heapAccessShiftForWebGLHeap(heap) {
      return 31 - Math.clz32(heap.BYTES_PER_ELEMENT);
    }
  
  function emscriptenWebGLGetTexPixelData(type, format, width, height, pixels, internalFormat) {
      var heap = heapObjectForWebGLType(type);
      var shift = heapAccessShiftForWebGLHeap(heap);
      var byteSize = 1<<shift;
      var sizePerPixel = colorChannelsInGlTextureFormat(format) * byteSize;
      var bytes = computeUnpackAlignedImageSize(width, height, sizePerPixel, GL.unpackAlignment);
      return heap.subarray(pixels >> shift, pixels + bytes >> shift);
    }
  
  
  
  /** @suppress {duplicate } */
  function _glReadPixels(x, y, width, height, format, type, pixels) {
      if (GL.currentContext.version >= 2) { // WebGL 2 provides new garbage-free entry points to call to WebGL. Use those always when possible.
        if (GLctx.currentPixelPackBufferBinding) {
          GLctx.readPixels(x, y, width, height, format, type, pixels);
        } else {
          var heap = heapObjectForWebGLType(type);
          GLctx.readPixels(x, y, width, height, format, type, heap, pixels >> heapAccessShiftForWebGLHeap(heap));
        }
        return;
      }
      var pixelData = emscriptenWebGLGetTexPixelData(type, format, width, height, pixels, format);
      if (!pixelData) {
        GL.recordError(0x500/*GL_INVALID_ENUM*/);
        return;
      }
      GLctx.readPixels(x, y, width, height, format, type, pixelData);
    }
  var _emscripten_glReadPixels = _glReadPixels;

  /** @suppress {duplicate } */
  function _glReleaseShaderCompiler() {
      // NOP (as allowed by GLES 2.0 spec)
    }
  var _emscripten_glReleaseShaderCompiler = _glReleaseShaderCompiler;

  /** @suppress {duplicate } */
  function _glRenderbufferStorage(x0, x1, x2, x3) { GLctx.renderbufferStorage(x0, x1, x2, x3) }
  var _emscripten_glRenderbufferStorage = _glRenderbufferStorage;

  /** @suppress {duplicate } */
  function _glRenderbufferStorageMultisample(x0, x1, x2, x3, x4) { GLctx.renderbufferStorageMultisample(x0, x1, x2, x3, x4) }
  var _emscripten_glRenderbufferStorageMultisample = _glRenderbufferStorageMultisample;

  /** @suppress {duplicate } */
  function _glResumeTransformFeedback() { GLctx.resumeTransformFeedback() }
  var _emscripten_glResumeTransformFeedback = _glResumeTransformFeedback;

  /** @suppress {duplicate } */
  function _glSampleCoverage(value, invert) {
      GLctx.sampleCoverage(value, !!invert);
    }
  var _emscripten_glSampleCoverage = _glSampleCoverage;

  /** @suppress {duplicate } */
  function _glSamplerParameterf(sampler, pname, param) {
      GLctx.samplerParameterf(GL.samplers[sampler], pname, param);
    }
  var _emscripten_glSamplerParameterf = _glSamplerParameterf;

  /** @suppress {duplicate } */
  function _glSamplerParameterfv(sampler, pname, params) {
      var param = HEAPF32[((params)>>2)];
      GLctx.samplerParameterf(GL.samplers[sampler], pname, param);
    }
  var _emscripten_glSamplerParameterfv = _glSamplerParameterfv;

  /** @suppress {duplicate } */
  function _glSamplerParameteri(sampler, pname, param) {
      GLctx.samplerParameteri(GL.samplers[sampler], pname, param);
    }
  var _emscripten_glSamplerParameteri = _glSamplerParameteri;

  /** @suppress {duplicate } */
  function _glSamplerParameteriv(sampler, pname, params) {
      var param = HEAP32[((params)>>2)];
      GLctx.samplerParameteri(GL.samplers[sampler], pname, param);
    }
  var _emscripten_glSamplerParameteriv = _glSamplerParameteriv;

  /** @suppress {duplicate } */
  function _glScissor(x0, x1, x2, x3) { GLctx.scissor(x0, x1, x2, x3) }
  var _emscripten_glScissor = _glScissor;

  /** @suppress {duplicate } */
  function _glShaderBinary(count, shaders, binaryformat, binary, length) {
      GL.recordError(0x500/*GL_INVALID_ENUM*/);
    }
  var _emscripten_glShaderBinary = _glShaderBinary;

  /** @suppress {duplicate } */
  function _glShaderSource(shader, count, string, length) {
      var source = GL.getSource(shader, count, string, length);
  
      GLctx.shaderSource(GL.shaders[shader], source);
    }
  var _emscripten_glShaderSource = _glShaderSource;

  /** @suppress {duplicate } */
  function _glStencilFunc(x0, x1, x2) { GLctx.stencilFunc(x0, x1, x2) }
  var _emscripten_glStencilFunc = _glStencilFunc;

  /** @suppress {duplicate } */
  function _glStencilFuncSeparate(x0, x1, x2, x3) { GLctx.stencilFuncSeparate(x0, x1, x2, x3) }
  var _emscripten_glStencilFuncSeparate = _glStencilFuncSeparate;

  /** @suppress {duplicate } */
  function _glStencilMask(x0) { GLctx.stencilMask(x0) }
  var _emscripten_glStencilMask = _glStencilMask;

  /** @suppress {duplicate } */
  function _glStencilMaskSeparate(x0, x1) { GLctx.stencilMaskSeparate(x0, x1) }
  var _emscripten_glStencilMaskSeparate = _glStencilMaskSeparate;

  /** @suppress {duplicate } */
  function _glStencilOp(x0, x1, x2) { GLctx.stencilOp(x0, x1, x2) }
  var _emscripten_glStencilOp = _glStencilOp;

  /** @suppress {duplicate } */
  function _glStencilOpSeparate(x0, x1, x2, x3) { GLctx.stencilOpSeparate(x0, x1, x2, x3) }
  var _emscripten_glStencilOpSeparate = _glStencilOpSeparate;

  
  
  
  /** @suppress {duplicate } */
  function _glTexImage2D(target, level, internalFormat, width, height, border, format, type, pixels) {
      if (GL.currentContext.version >= 2) {
        // WebGL 2 provides new garbage-free entry points to call to WebGL. Use those always when possible.
        if (GLctx.currentPixelUnpackBufferBinding) {
          GLctx.texImage2D(target, level, internalFormat, width, height, border, format, type, pixels);
        } else if (pixels) {
          var heap = heapObjectForWebGLType(type);
          GLctx.texImage2D(target, level, internalFormat, width, height, border, format, type, heap, pixels >> heapAccessShiftForWebGLHeap(heap));
        } else {
          GLctx.texImage2D(target, level, internalFormat, width, height, border, format, type, null);
        }
        return;
      }
      GLctx.texImage2D(target, level, internalFormat, width, height, border, format, type, pixels ? emscriptenWebGLGetTexPixelData(type, format, width, height, pixels, internalFormat) : null);
    }
  var _emscripten_glTexImage2D = _glTexImage2D;

  
  /** @suppress {duplicate } */
  function _glTexImage3D(target, level, internalFormat, width, height, depth, border, format, type, pixels) {
      if (GLctx.currentPixelUnpackBufferBinding) {
        GLctx.texImage3D(target, level, internalFormat, width, height, depth, border, format, type, pixels);
      } else if (pixels) {
        var heap = heapObjectForWebGLType(type);
        GLctx.texImage3D(target, level, internalFormat, width, height, depth, border, format, type, heap, pixels >> heapAccessShiftForWebGLHeap(heap));
      } else {
        GLctx.texImage3D(target, level, internalFormat, width, height, depth, border, format, type, null);
      }
    }
  var _emscripten_glTexImage3D = _glTexImage3D;

  /** @suppress {duplicate } */
  function _glTexParameterf(x0, x1, x2) { GLctx.texParameterf(x0, x1, x2) }
  var _emscripten_glTexParameterf = _glTexParameterf;

  /** @suppress {duplicate } */
  function _glTexParameterfv(target, pname, params) {
      var param = HEAPF32[((params)>>2)];
      GLctx.texParameterf(target, pname, param);
    }
  var _emscripten_glTexParameterfv = _glTexParameterfv;

  /** @suppress {duplicate } */
  function _glTexParameteri(x0, x1, x2) { GLctx.texParameteri(x0, x1, x2) }
  var _emscripten_glTexParameteri = _glTexParameteri;

  /** @suppress {duplicate } */
  function _glTexParameteriv(target, pname, params) {
      var param = HEAP32[((params)>>2)];
      GLctx.texParameteri(target, pname, param);
    }
  var _emscripten_glTexParameteriv = _glTexParameteriv;

  /** @suppress {duplicate } */
  function _glTexStorage2D(x0, x1, x2, x3, x4) { GLctx.texStorage2D(x0, x1, x2, x3, x4) }
  var _emscripten_glTexStorage2D = _glTexStorage2D;

  /** @suppress {duplicate } */
  function _glTexStorage3D(x0, x1, x2, x3, x4, x5) { GLctx.texStorage3D(x0, x1, x2, x3, x4, x5) }
  var _emscripten_glTexStorage3D = _glTexStorage3D;

  
  
  
  /** @suppress {duplicate } */
  function _glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels) {
      if (GL.currentContext.version >= 2) {
        // WebGL 2 provides new garbage-free entry points to call to WebGL. Use those always when possible.
        if (GLctx.currentPixelUnpackBufferBinding) {
          GLctx.texSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
        } else if (pixels) {
          var heap = heapObjectForWebGLType(type);
          GLctx.texSubImage2D(target, level, xoffset, yoffset, width, height, format, type, heap, pixels >> heapAccessShiftForWebGLHeap(heap));
        } else {
          GLctx.texSubImage2D(target, level, xoffset, yoffset, width, height, format, type, null);
        }
        return;
      }
      var pixelData = null;
      if (pixels) pixelData = emscriptenWebGLGetTexPixelData(type, format, width, height, pixels, 0);
      GLctx.texSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixelData);
    }
  var _emscripten_glTexSubImage2D = _glTexSubImage2D;

  
  /** @suppress {duplicate } */
  function _glTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels) {
      if (GLctx.currentPixelUnpackBufferBinding) {
        GLctx.texSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);
      } else if (pixels) {
        var heap = heapObjectForWebGLType(type);
        GLctx.texSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, heap, pixels >> heapAccessShiftForWebGLHeap(heap));
      } else {
        GLctx.texSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, null);
      }
    }
  var _emscripten_glTexSubImage3D = _glTexSubImage3D;

  /** @suppress {duplicate } */
  function _glTransformFeedbackVaryings(program, count, varyings, bufferMode) {
      program = GL.programs[program];
      var vars = [];
      for (var i = 0; i < count; i++)
        vars.push(UTF8ToString(HEAP32[(((varyings)+(i*4))>>2)]));
  
      GLctx.transformFeedbackVaryings(program, vars, bufferMode);
    }
  var _emscripten_glTransformFeedbackVaryings = _glTransformFeedbackVaryings;

  
  /** @suppress {duplicate } */
  function _glUniform1f(location, v0) {
      GLctx.uniform1f(webglGetUniformLocation(location), v0);
    }
  var _emscripten_glUniform1f = _glUniform1f;

  
  var miniTempWebGLFloatBuffers = [];
  
  /** @suppress {duplicate } */
  function _glUniform1fv(location, count, value) {
  
      if (GL.currentContext.version >= 2) { // WebGL 2 provides new garbage-free entry points to call to WebGL. Use those always when possible.
        count && GLctx.uniform1fv(webglGetUniformLocation(location), HEAPF32, value>>2, count);
        return;
      }
  
      if (count <= 288) {
        // avoid allocation when uploading few enough uniforms
        var view = miniTempWebGLFloatBuffers[count-1];
        for (var i = 0; i < count; ++i) {
          view[i] = HEAPF32[(((value)+(4*i))>>2)];
        }
      } else
      {
        var view = HEAPF32.subarray((value)>>2, (value+count*4)>>2);
      }
      GLctx.uniform1fv(webglGetUniformLocation(location), view);
    }
  var _emscripten_glUniform1fv = _glUniform1fv;

  
  /** @suppress {duplicate } */
  function _glUniform1i(location, v0) {
      GLctx.uniform1i(webglGetUniformLocation(location), v0);
    }
  var _emscripten_glUniform1i = _glUniform1i;

  
  var miniTempWebGLIntBuffers = [];
  
  /** @suppress {duplicate } */
  function _glUniform1iv(location, count, value) {
  
      if (GL.currentContext.version >= 2) { // WebGL 2 provides new garbage-free entry points to call to WebGL. Use those always when possible.
        count && GLctx.uniform1iv(webglGetUniformLocation(location), HEAP32, value>>2, count);
        return;
      }
  
      if (count <= 288) {
        // avoid allocation when uploading few enough uniforms
        var view = miniTempWebGLIntBuffers[count-1];
        for (var i = 0; i < count; ++i) {
          view[i] = HEAP32[(((value)+(4*i))>>2)];
        }
      } else
      {
        var view = HEAP32.subarray((value)>>2, (value+count*4)>>2);
      }
      GLctx.uniform1iv(webglGetUniformLocation(location), view);
    }
  var _emscripten_glUniform1iv = _glUniform1iv;

  /** @suppress {duplicate } */
  function _glUniform1ui(location, v0) {
      GLctx.uniform1ui(webglGetUniformLocation(location), v0);
    }
  var _emscripten_glUniform1ui = _glUniform1ui;

  /** @suppress {duplicate } */
  function _glUniform1uiv(location, count, value) {
      count && GLctx.uniform1uiv(webglGetUniformLocation(location), HEAPU32, value>>2, count);
    }
  var _emscripten_glUniform1uiv = _glUniform1uiv;

  
  /** @suppress {duplicate } */
  function _glUniform2f(location, v0, v1) {
      GLctx.uniform2f(webglGetUniformLocation(location), v0, v1);
    }
  var _emscripten_glUniform2f = _glUniform2f;

  
  
  /** @suppress {duplicate } */
  function _glUniform2fv(location, count, value) {
  
      if (GL.currentContext.version >= 2) { // WebGL 2 provides new garbage-free entry points to call to WebGL. Use those always when possible.
        count && GLctx.uniform2fv(webglGetUniformLocation(location), HEAPF32, value>>2, count*2);
        return;
      }
  
      if (count <= 144) {
        // avoid allocation when uploading few enough uniforms
        var view = miniTempWebGLFloatBuffers[2*count-1];
        for (var i = 0; i < 2*count; i += 2) {
          view[i] = HEAPF32[(((value)+(4*i))>>2)];
          view[i+1] = HEAPF32[(((value)+(4*i+4))>>2)];
        }
      } else
      {
        var view = HEAPF32.subarray((value)>>2, (value+count*8)>>2);
      }
      GLctx.uniform2fv(webglGetUniformLocation(location), view);
    }
  var _emscripten_glUniform2fv = _glUniform2fv;

  
  /** @suppress {duplicate } */
  function _glUniform2i(location, v0, v1) {
      GLctx.uniform2i(webglGetUniformLocation(location), v0, v1);
    }
  var _emscripten_glUniform2i = _glUniform2i;

  
  
  /** @suppress {duplicate } */
  function _glUniform2iv(location, count, value) {
  
      if (GL.currentContext.version >= 2) { // WebGL 2 provides new garbage-free entry points to call to WebGL. Use those always when possible.
        count && GLctx.uniform2iv(webglGetUniformLocation(location), HEAP32, value>>2, count*2);
        return;
      }
  
      if (count <= 144) {
        // avoid allocation when uploading few enough uniforms
        var view = miniTempWebGLIntBuffers[2*count-1];
        for (var i = 0; i < 2*count; i += 2) {
          view[i] = HEAP32[(((value)+(4*i))>>2)];
          view[i+1] = HEAP32[(((value)+(4*i+4))>>2)];
        }
      } else
      {
        var view = HEAP32.subarray((value)>>2, (value+count*8)>>2);
      }
      GLctx.uniform2iv(webglGetUniformLocation(location), view);
    }
  var _emscripten_glUniform2iv = _glUniform2iv;

  /** @suppress {duplicate } */
  function _glUniform2ui(location, v0, v1) {
      GLctx.uniform2ui(webglGetUniformLocation(location), v0, v1);
    }
  var _emscripten_glUniform2ui = _glUniform2ui;

  /** @suppress {duplicate } */
  function _glUniform2uiv(location, count, value) {
      count && GLctx.uniform2uiv(webglGetUniformLocation(location), HEAPU32, value>>2, count*2);
    }
  var _emscripten_glUniform2uiv = _glUniform2uiv;

  
  /** @suppress {duplicate } */
  function _glUniform3f(location, v0, v1, v2) {
      GLctx.uniform3f(webglGetUniformLocation(location), v0, v1, v2);
    }
  var _emscripten_glUniform3f = _glUniform3f;

  
  
  /** @suppress {duplicate } */
  function _glUniform3fv(location, count, value) {
  
      if (GL.currentContext.version >= 2) { // WebGL 2 provides new garbage-free entry points to call to WebGL. Use those always when possible.
        count && GLctx.uniform3fv(webglGetUniformLocation(location), HEAPF32, value>>2, count*3);
        return;
      }
  
      if (count <= 96) {
        // avoid allocation when uploading few enough uniforms
        var view = miniTempWebGLFloatBuffers[3*count-1];
        for (var i = 0; i < 3*count; i += 3) {
          view[i] = HEAPF32[(((value)+(4*i))>>2)];
          view[i+1] = HEAPF32[(((value)+(4*i+4))>>2)];
          view[i+2] = HEAPF32[(((value)+(4*i+8))>>2)];
        }
      } else
      {
        var view = HEAPF32.subarray((value)>>2, (value+count*12)>>2);
      }
      GLctx.uniform3fv(webglGetUniformLocation(location), view);
    }
  var _emscripten_glUniform3fv = _glUniform3fv;

  
  /** @suppress {duplicate } */
  function _glUniform3i(location, v0, v1, v2) {
      GLctx.uniform3i(webglGetUniformLocation(location), v0, v1, v2);
    }
  var _emscripten_glUniform3i = _glUniform3i;

  
  
  /** @suppress {duplicate } */
  function _glUniform3iv(location, count, value) {
  
      if (GL.currentContext.version >= 2) { // WebGL 2 provides new garbage-free entry points to call to WebGL. Use those always when possible.
        count && GLctx.uniform3iv(webglGetUniformLocation(location), HEAP32, value>>2, count*3);
        return;
      }
  
      if (count <= 96) {
        // avoid allocation when uploading few enough uniforms
        var view = miniTempWebGLIntBuffers[3*count-1];
        for (var i = 0; i < 3*count; i += 3) {
          view[i] = HEAP32[(((value)+(4*i))>>2)];
          view[i+1] = HEAP32[(((value)+(4*i+4))>>2)];
          view[i+2] = HEAP32[(((value)+(4*i+8))>>2)];
        }
      } else
      {
        var view = HEAP32.subarray((value)>>2, (value+count*12)>>2);
      }
      GLctx.uniform3iv(webglGetUniformLocation(location), view);
    }
  var _emscripten_glUniform3iv = _glUniform3iv;

  /** @suppress {duplicate } */
  function _glUniform3ui(location, v0, v1, v2) {
      GLctx.uniform3ui(webglGetUniformLocation(location), v0, v1, v2);
    }
  var _emscripten_glUniform3ui = _glUniform3ui;

  /** @suppress {duplicate } */
  function _glUniform3uiv(location, count, value) {
      count && GLctx.uniform3uiv(webglGetUniformLocation(location), HEAPU32, value>>2, count*3);
    }
  var _emscripten_glUniform3uiv = _glUniform3uiv;

  
  /** @suppress {duplicate } */
  function _glUniform4f(location, v0, v1, v2, v3) {
      GLctx.uniform4f(webglGetUniformLocation(location), v0, v1, v2, v3);
    }
  var _emscripten_glUniform4f = _glUniform4f;

  
  
  /** @suppress {duplicate } */
  function _glUniform4fv(location, count, value) {
  
      if (GL.currentContext.version >= 2) { // WebGL 2 provides new garbage-free entry points to call to WebGL. Use those always when possible.
        count && GLctx.uniform4fv(webglGetUniformLocation(location), HEAPF32, value>>2, count*4);
        return;
      }
  
      if (count <= 72) {
        // avoid allocation when uploading few enough uniforms
        var view = miniTempWebGLFloatBuffers[4*count-1];
        // hoist the heap out of the loop for size and for pthreads+growth.
        var heap = HEAPF32;
        value >>= 2;
        for (var i = 0; i < 4 * count; i += 4) {
          var dst = value + i;
          view[i] = heap[dst];
          view[i + 1] = heap[dst + 1];
          view[i + 2] = heap[dst + 2];
          view[i + 3] = heap[dst + 3];
        }
      } else
      {
        var view = HEAPF32.subarray((value)>>2, (value+count*16)>>2);
      }
      GLctx.uniform4fv(webglGetUniformLocation(location), view);
    }
  var _emscripten_glUniform4fv = _glUniform4fv;

  
  /** @suppress {duplicate } */
  function _glUniform4i(location, v0, v1, v2, v3) {
      GLctx.uniform4i(webglGetUniformLocation(location), v0, v1, v2, v3);
    }
  var _emscripten_glUniform4i = _glUniform4i;

  
  
  /** @suppress {duplicate } */
  function _glUniform4iv(location, count, value) {
  
      if (GL.currentContext.version >= 2) { // WebGL 2 provides new garbage-free entry points to call to WebGL. Use those always when possible.
        count && GLctx.uniform4iv(webglGetUniformLocation(location), HEAP32, value>>2, count*4);
        return;
      }
  
      if (count <= 72) {
        // avoid allocation when uploading few enough uniforms
        var view = miniTempWebGLIntBuffers[4*count-1];
        for (var i = 0; i < 4*count; i += 4) {
          view[i] = HEAP32[(((value)+(4*i))>>2)];
          view[i+1] = HEAP32[(((value)+(4*i+4))>>2)];
          view[i+2] = HEAP32[(((value)+(4*i+8))>>2)];
          view[i+3] = HEAP32[(((value)+(4*i+12))>>2)];
        }
      } else
      {
        var view = HEAP32.subarray((value)>>2, (value+count*16)>>2);
      }
      GLctx.uniform4iv(webglGetUniformLocation(location), view);
    }
  var _emscripten_glUniform4iv = _glUniform4iv;

  /** @suppress {duplicate } */
  function _glUniform4ui(location, v0, v1, v2, v3) {
      GLctx.uniform4ui(webglGetUniformLocation(location), v0, v1, v2, v3);
    }
  var _emscripten_glUniform4ui = _glUniform4ui;

  /** @suppress {duplicate } */
  function _glUniform4uiv(location, count, value) {
      count && GLctx.uniform4uiv(webglGetUniformLocation(location), HEAPU32, value>>2, count*4);
    }
  var _emscripten_glUniform4uiv = _glUniform4uiv;

  /** @suppress {duplicate } */
  function _glUniformBlockBinding(program, uniformBlockIndex, uniformBlockBinding) {
      program = GL.programs[program];
  
      GLctx.uniformBlockBinding(program, uniformBlockIndex, uniformBlockBinding);
    }
  var _emscripten_glUniformBlockBinding = _glUniformBlockBinding;

  
  
  /** @suppress {duplicate } */
  function _glUniformMatrix2fv(location, count, transpose, value) {
  
      if (GL.currentContext.version >= 2) { // WebGL 2 provides new garbage-free entry points to call to WebGL. Use those always when possible.
        count && GLctx.uniformMatrix2fv(webglGetUniformLocation(location), !!transpose, HEAPF32, value>>2, count*4);
        return;
      }
  
      if (count <= 72) {
        // avoid allocation when uploading few enough uniforms
        var view = miniTempWebGLFloatBuffers[4*count-1];
        for (var i = 0; i < 4*count; i += 4) {
          view[i] = HEAPF32[(((value)+(4*i))>>2)];
          view[i+1] = HEAPF32[(((value)+(4*i+4))>>2)];
          view[i+2] = HEAPF32[(((value)+(4*i+8))>>2)];
          view[i+3] = HEAPF32[(((value)+(4*i+12))>>2)];
        }
      } else
      {
        var view = HEAPF32.subarray((value)>>2, (value+count*16)>>2);
      }
      GLctx.uniformMatrix2fv(webglGetUniformLocation(location), !!transpose, view);
    }
  var _emscripten_glUniformMatrix2fv = _glUniformMatrix2fv;

  /** @suppress {duplicate } */
  function _glUniformMatrix2x3fv(location, count, transpose, value) {
      count && GLctx.uniformMatrix2x3fv(webglGetUniformLocation(location), !!transpose, HEAPF32, value>>2, count*6);
    }
  var _emscripten_glUniformMatrix2x3fv = _glUniformMatrix2x3fv;

  /** @suppress {duplicate } */
  function _glUniformMatrix2x4fv(location, count, transpose, value) {
      count && GLctx.uniformMatrix2x4fv(webglGetUniformLocation(location), !!transpose, HEAPF32, value>>2, count*8);
    }
  var _emscripten_glUniformMatrix2x4fv = _glUniformMatrix2x4fv;

  
  
  /** @suppress {duplicate } */
  function _glUniformMatrix3fv(location, count, transpose, value) {
  
      if (GL.currentContext.version >= 2) { // WebGL 2 provides new garbage-free entry points to call to WebGL. Use those always when possible.
        count && GLctx.uniformMatrix3fv(webglGetUniformLocation(location), !!transpose, HEAPF32, value>>2, count*9);
        return;
      }
  
      if (count <= 32) {
        // avoid allocation when uploading few enough uniforms
        var view = miniTempWebGLFloatBuffers[9*count-1];
        for (var i = 0; i < 9*count; i += 9) {
          view[i] = HEAPF32[(((value)+(4*i))>>2)];
          view[i+1] = HEAPF32[(((value)+(4*i+4))>>2)];
          view[i+2] = HEAPF32[(((value)+(4*i+8))>>2)];
          view[i+3] = HEAPF32[(((value)+(4*i+12))>>2)];
          view[i+4] = HEAPF32[(((value)+(4*i+16))>>2)];
          view[i+5] = HEAPF32[(((value)+(4*i+20))>>2)];
          view[i+6] = HEAPF32[(((value)+(4*i+24))>>2)];
          view[i+7] = HEAPF32[(((value)+(4*i+28))>>2)];
          view[i+8] = HEAPF32[(((value)+(4*i+32))>>2)];
        }
      } else
      {
        var view = HEAPF32.subarray((value)>>2, (value+count*36)>>2);
      }
      GLctx.uniformMatrix3fv(webglGetUniformLocation(location), !!transpose, view);
    }
  var _emscripten_glUniformMatrix3fv = _glUniformMatrix3fv;

  /** @suppress {duplicate } */
  function _glUniformMatrix3x2fv(location, count, transpose, value) {
      count && GLctx.uniformMatrix3x2fv(webglGetUniformLocation(location), !!transpose, HEAPF32, value>>2, count*6);
    }
  var _emscripten_glUniformMatrix3x2fv = _glUniformMatrix3x2fv;

  /** @suppress {duplicate } */
  function _glUniformMatrix3x4fv(location, count, transpose, value) {
      count && GLctx.uniformMatrix3x4fv(webglGetUniformLocation(location), !!transpose, HEAPF32, value>>2, count*12);
    }
  var _emscripten_glUniformMatrix3x4fv = _glUniformMatrix3x4fv;

  
  
  /** @suppress {duplicate } */
  function _glUniformMatrix4fv(location, count, transpose, value) {
  
      if (GL.currentContext.version >= 2) { // WebGL 2 provides new garbage-free entry points to call to WebGL. Use those always when possible.
        count && GLctx.uniformMatrix4fv(webglGetUniformLocation(location), !!transpose, HEAPF32, value>>2, count*16);
        return;
      }
  
      if (count <= 18) {
        // avoid allocation when uploading few enough uniforms
        var view = miniTempWebGLFloatBuffers[16*count-1];
        // hoist the heap out of the loop for size and for pthreads+growth.
        var heap = HEAPF32;
        value >>= 2;
        for (var i = 0; i < 16 * count; i += 16) {
          var dst = value + i;
          view[i] = heap[dst];
          view[i + 1] = heap[dst + 1];
          view[i + 2] = heap[dst + 2];
          view[i + 3] = heap[dst + 3];
          view[i + 4] = heap[dst + 4];
          view[i + 5] = heap[dst + 5];
          view[i + 6] = heap[dst + 6];
          view[i + 7] = heap[dst + 7];
          view[i + 8] = heap[dst + 8];
          view[i + 9] = heap[dst + 9];
          view[i + 10] = heap[dst + 10];
          view[i + 11] = heap[dst + 11];
          view[i + 12] = heap[dst + 12];
          view[i + 13] = heap[dst + 13];
          view[i + 14] = heap[dst + 14];
          view[i + 15] = heap[dst + 15];
        }
      } else
      {
        var view = HEAPF32.subarray((value)>>2, (value+count*64)>>2);
      }
      GLctx.uniformMatrix4fv(webglGetUniformLocation(location), !!transpose, view);
    }
  var _emscripten_glUniformMatrix4fv = _glUniformMatrix4fv;

  /** @suppress {duplicate } */
  function _glUniformMatrix4x2fv(location, count, transpose, value) {
      count && GLctx.uniformMatrix4x2fv(webglGetUniformLocation(location), !!transpose, HEAPF32, value>>2, count*8);
    }
  var _emscripten_glUniformMatrix4x2fv = _glUniformMatrix4x2fv;

  /** @suppress {duplicate } */
  function _glUniformMatrix4x3fv(location, count, transpose, value) {
      count && GLctx.uniformMatrix4x3fv(webglGetUniformLocation(location), !!transpose, HEAPF32, value>>2, count*12);
    }
  var _emscripten_glUniformMatrix4x3fv = _glUniformMatrix4x3fv;

  
  
  
  /** @suppress {duplicate } */
  function _glUnmapBuffer(target) {
      if (!emscriptenWebGLValidateMapBufferTarget(target)) {
        GL.recordError(0x500/*GL_INVALID_ENUM*/);
        err('GL_INVALID_ENUM in glUnmapBuffer');
        return 0;
      }
  
      var buffer = emscriptenWebGLGetBufferBinding(target);
      var mapping = GL.mappedBuffers[buffer];
      if (!mapping || !mapping.mem) {
        GL.recordError(0x502 /* GL_INVALID_OPERATION */);
        err('buffer was never mapped in glUnmapBuffer');
        return 0;
      }
  
      if (!(mapping.access & 0x10)) /* GL_MAP_FLUSH_EXPLICIT_BIT */
        if (GL.currentContext.version >= 2) { // WebGL 2 provides new garbage-free entry points to call to WebGL. Use those always when possible.
          GLctx.bufferSubData(target, mapping.offset, HEAPU8, mapping.mem, mapping.length);
        } else {
          GLctx.bufferSubData(target, mapping.offset, HEAPU8.subarray(mapping.mem, mapping.mem+mapping.length));
        }
      _free(mapping.mem);
      mapping.mem = 0;
      return 1;
    }
  var _emscripten_glUnmapBuffer = _glUnmapBuffer;

  /** @suppress {duplicate } */
  function _glUseProgram(program) {
      program = GL.programs[program];
      GLctx.useProgram(program);
      // Record the currently active program so that we can access the uniform
      // mapping table of that program.
      GLctx.currentProgram = program;
    }
  var _emscripten_glUseProgram = _glUseProgram;

  /** @suppress {duplicate } */
  function _glValidateProgram(program) {
      GLctx.validateProgram(GL.programs[program]);
    }
  var _emscripten_glValidateProgram = _glValidateProgram;

  /** @suppress {duplicate } */
  function _glVertexAttrib1f(x0, x1) { GLctx.vertexAttrib1f(x0, x1) }
  var _emscripten_glVertexAttrib1f = _glVertexAttrib1f;

  /** @suppress {duplicate } */
  function _glVertexAttrib1fv(index, v) {
  
      GLctx.vertexAttrib1f(index, HEAPF32[v>>2]);
    }
  var _emscripten_glVertexAttrib1fv = _glVertexAttrib1fv;

  /** @suppress {duplicate } */
  function _glVertexAttrib2f(x0, x1, x2) { GLctx.vertexAttrib2f(x0, x1, x2) }
  var _emscripten_glVertexAttrib2f = _glVertexAttrib2f;

  /** @suppress {duplicate } */
  function _glVertexAttrib2fv(index, v) {
  
      GLctx.vertexAttrib2f(index, HEAPF32[v>>2], HEAPF32[v+4>>2]);
    }
  var _emscripten_glVertexAttrib2fv = _glVertexAttrib2fv;

  /** @suppress {duplicate } */
  function _glVertexAttrib3f(x0, x1, x2, x3) { GLctx.vertexAttrib3f(x0, x1, x2, x3) }
  var _emscripten_glVertexAttrib3f = _glVertexAttrib3f;

  /** @suppress {duplicate } */
  function _glVertexAttrib3fv(index, v) {
  
      GLctx.vertexAttrib3f(index, HEAPF32[v>>2], HEAPF32[v+4>>2], HEAPF32[v+8>>2]);
    }
  var _emscripten_glVertexAttrib3fv = _glVertexAttrib3fv;

  /** @suppress {duplicate } */
  function _glVertexAttrib4f(x0, x1, x2, x3, x4) { GLctx.vertexAttrib4f(x0, x1, x2, x3, x4) }
  var _emscripten_glVertexAttrib4f = _glVertexAttrib4f;

  /** @suppress {duplicate } */
  function _glVertexAttrib4fv(index, v) {
  
      GLctx.vertexAttrib4f(index, HEAPF32[v>>2], HEAPF32[v+4>>2], HEAPF32[v+8>>2], HEAPF32[v+12>>2]);
    }
  var _emscripten_glVertexAttrib4fv = _glVertexAttrib4fv;

  /** @suppress {duplicate } */
  function _glVertexAttribDivisor(index, divisor) {
      GLctx.vertexAttribDivisor(index, divisor);
    }
  var _emscripten_glVertexAttribDivisor = _glVertexAttribDivisor;

  
  /** @suppress {duplicate } */
  var _glVertexAttribDivisorANGLE = _glVertexAttribDivisor;
  var _emscripten_glVertexAttribDivisorANGLE = _glVertexAttribDivisorANGLE;

  
  /** @suppress {duplicate } */
  var _glVertexAttribDivisorARB = _glVertexAttribDivisor;
  var _emscripten_glVertexAttribDivisorARB = _glVertexAttribDivisorARB;

  
  /** @suppress {duplicate } */
  var _glVertexAttribDivisorEXT = _glVertexAttribDivisor;
  var _emscripten_glVertexAttribDivisorEXT = _glVertexAttribDivisorEXT;

  
  /** @suppress {duplicate } */
  var _glVertexAttribDivisorNV = _glVertexAttribDivisor;
  var _emscripten_glVertexAttribDivisorNV = _glVertexAttribDivisorNV;

  /** @suppress {duplicate } */
  function _glVertexAttribI4i(x0, x1, x2, x3, x4) { GLctx.vertexAttribI4i(x0, x1, x2, x3, x4) }
  var _emscripten_glVertexAttribI4i = _glVertexAttribI4i;

  /** @suppress {duplicate } */
  function _glVertexAttribI4iv(index, v) {
      GLctx.vertexAttribI4i(index, HEAP32[v>>2], HEAP32[v+4>>2], HEAP32[v+8>>2], HEAP32[v+12>>2]);
    }
  var _emscripten_glVertexAttribI4iv = _glVertexAttribI4iv;

  /** @suppress {duplicate } */
  function _glVertexAttribI4ui(x0, x1, x2, x3, x4) { GLctx.vertexAttribI4ui(x0, x1, x2, x3, x4) }
  var _emscripten_glVertexAttribI4ui = _glVertexAttribI4ui;

  /** @suppress {duplicate } */
  function _glVertexAttribI4uiv(index, v) {
      GLctx.vertexAttribI4ui(index, HEAPU32[v>>2], HEAPU32[v+4>>2], HEAPU32[v+8>>2], HEAPU32[v+12>>2]);
    }
  var _emscripten_glVertexAttribI4uiv = _glVertexAttribI4uiv;

  /** @suppress {duplicate } */
  function _glVertexAttribIPointer(index, size, type, stride, ptr) {
      var cb = GL.currentContext.clientBuffers[index];
      if (!GLctx.currentArrayBufferBinding) {
        cb.size = size;
        cb.type = type;
        cb.normalized = false;
        cb.stride = stride;
        cb.ptr = ptr;
        cb.clientside = true;
        cb.vertexAttribPointerAdaptor = function(index, size, type, normalized, stride, ptr) {
          this.vertexAttribIPointer(index, size, type, stride, ptr);
        };
        return;
      }
      cb.clientside = false;
      GLctx.vertexAttribIPointer(index, size, type, stride, ptr);
    }
  var _emscripten_glVertexAttribIPointer = _glVertexAttribIPointer;

  /** @suppress {duplicate } */
  function _glVertexAttribPointer(index, size, type, normalized, stride, ptr) {
      var cb = GL.currentContext.clientBuffers[index];
      if (!GLctx.currentArrayBufferBinding) {
        cb.size = size;
        cb.type = type;
        cb.normalized = normalized;
        cb.stride = stride;
        cb.ptr = ptr;
        cb.clientside = true;
        cb.vertexAttribPointerAdaptor = function(index, size, type, normalized, stride, ptr) {
          this.vertexAttribPointer(index, size, type, normalized, stride, ptr);
        };
        return;
      }
      cb.clientside = false;
      GLctx.vertexAttribPointer(index, size, type, !!normalized, stride, ptr);
    }
  var _emscripten_glVertexAttribPointer = _glVertexAttribPointer;

  /** @suppress {duplicate } */
  function _glViewport(x0, x1, x2, x3) { GLctx.viewport(x0, x1, x2, x3) }
  var _emscripten_glViewport = _glViewport;

  /** @suppress {duplicate } */
  function _glWaitSync(sync, flags, timeout_low, timeout_high) {
      // See WebGL2 vs GLES3 difference on GL_TIMEOUT_IGNORED above (https://www.khronos.org/registry/webgl/specs/latest/2.0/#5.15)
      var timeout = convertI32PairToI53(timeout_low, timeout_high);
      GLctx.waitSync(GL.syncs[sync], flags, timeout);
    }
  var _emscripten_glWaitSync = _glWaitSync;

  var _emscripten_has_asyncify = () => 0;

  var _emscripten_memcpy_big = (dest, src, num) => HEAPU8.copyWithin(dest, src, src + num);

  
  
  
  
  
  
  
  function doRequestFullscreen(target, strategy) {
      if (!JSEvents.fullscreenEnabled()) return -1;
      target = findEventTarget(target);
      if (!target) return -4;
  
      if (!target.requestFullscreen
        && !target.webkitRequestFullscreen
        ) {
        return -3;
      }
  
      var canPerformRequests = JSEvents.canPerformEventHandlerRequests();
  
      // Queue this function call if we're not currently in an event handler and the user saw it appropriate to do so.
      if (!canPerformRequests) {
        if (strategy.deferUntilInEventHandler) {
          JSEvents.deferCall(JSEvents_requestFullscreen, 1 /* priority over pointer lock */, [target, strategy]);
          return 1;
        }
        return -2;
      }
  
      return JSEvents_requestFullscreen(target, strategy);
    }
  
  
  function _emscripten_request_fullscreen_strategy(target, deferUntilInEventHandler, fullscreenStrategy) {
      var strategy = {
        scaleMode: HEAP32[((fullscreenStrategy)>>2)],
        canvasResolutionScaleMode: HEAP32[(((fullscreenStrategy)+(4))>>2)],
        filteringMode: HEAP32[(((fullscreenStrategy)+(8))>>2)],
        deferUntilInEventHandler,
        canvasResizedCallback: HEAP32[(((fullscreenStrategy)+(12))>>2)],
        canvasResizedCallbackUserData: HEAP32[(((fullscreenStrategy)+(16))>>2)]
      };
  
      return doRequestFullscreen(target, strategy);
    }

  
  
  function _emscripten_request_pointerlock(target, deferUntilInEventHandler) {
      target = findEventTarget(target);
      if (!target) return -4;
      if (!target.requestPointerLock
        ) {
        return -1;
      }
  
      var canPerformRequests = JSEvents.canPerformEventHandlerRequests();
  
      // Queue this function call if we're not currently in an event handler and the user saw it appropriate to do so.
      if (!canPerformRequests) {
        if (deferUntilInEventHandler) {
          JSEvents.deferCall(requestPointerLock, 2 /* priority below fullscreen */, [target]);
          return 1;
        }
        return -2;
      }
  
      return requestPointerLock(target);
    }

  
  var growMemory = (size) => {
      var b = wasmMemory.buffer;
      var pages = (size - b.byteLength + 65535) >>> 16;
      try {
        // round size grow request up to wasm page size (fixed 64KB per spec)
        wasmMemory.grow(pages); // .grow() takes a delta compared to the previous size
        updateMemoryViews();
        return 1 /*success*/;
      } catch(e) {
        err(`growMemory: Attempted to grow heap from ${b.byteLength} bytes to ${size} bytes, but got error: ${e}`);
      }
      // implicit 0 return to save code size (caller will cast "undefined" into 0
      // anyhow)
    };
  var _emscripten_resize_heap = (requestedSize) => {
      var oldSize = HEAPU8.length;
      // With CAN_ADDRESS_2GB or MEMORY64, pointers are already unsigned.
      requestedSize >>>= 0;
      // With multithreaded builds, races can happen (another thread might increase the size
      // in between), so return a failure, and let the caller retry.
      assert(requestedSize > oldSize);
  
      // Memory resize rules:
      // 1.  Always increase heap size to at least the requested size, rounded up
      //     to next page multiple.
      // 2a. If MEMORY_GROWTH_LINEAR_STEP == -1, excessively resize the heap
      //     geometrically: increase the heap size according to
      //     MEMORY_GROWTH_GEOMETRIC_STEP factor (default +20%), At most
      //     overreserve by MEMORY_GROWTH_GEOMETRIC_CAP bytes (default 96MB).
      // 2b. If MEMORY_GROWTH_LINEAR_STEP != -1, excessively resize the heap
      //     linearly: increase the heap size by at least
      //     MEMORY_GROWTH_LINEAR_STEP bytes.
      // 3.  Max size for the heap is capped at 2048MB-WASM_PAGE_SIZE, or by
      //     MAXIMUM_MEMORY, or by ASAN limit, depending on which is smallest
      // 4.  If we were unable to allocate as much memory, it may be due to
      //     over-eager decision to excessively reserve due to (3) above.
      //     Hence if an allocation fails, cut down on the amount of excess
      //     growth, in an attempt to succeed to perform a smaller allocation.
  
      // A limit is set for how much we can grow. We should not exceed that
      // (the wasm binary specifies it, so if we tried, we'd fail anyhow).
      var maxHeapSize = getHeapMax();
      if (requestedSize > maxHeapSize) {
        err(`Cannot enlarge memory, asked to go up to ${requestedSize} bytes, but the limit is ${maxHeapSize} bytes!`);
        return false;
      }
  
      var alignUp = (x, multiple) => x + (multiple - x % multiple) % multiple;
  
      // Loop through potential heap size increases. If we attempt a too eager
      // reservation that fails, cut down on the attempted size and reserve a
      // smaller bump instead. (max 3 times, chosen somewhat arbitrarily)
      for (var cutDown = 1; cutDown <= 4; cutDown *= 2) {
        var overGrownHeapSize = oldSize * (1 + 0.2 / cutDown); // ensure geometric growth
        // but limit overreserving (default to capping at +96MB overgrowth at most)
        overGrownHeapSize = Math.min(overGrownHeapSize, requestedSize + 100663296 );
  
        var newSize = Math.min(maxHeapSize, alignUp(Math.max(requestedSize, overGrownHeapSize), 65536));
  
        var replacement = growMemory(newSize);
        if (replacement) {
  
          return true;
        }
      }
      err(`Failed to grow the heap from ${oldSize} bytes to ${newSize} bytes, not enough memory!`);
      return false;
    };

  function _emscripten_sample_gamepad_data() {
      return (JSEvents.lastGamepadState = (navigator.getGamepads ? navigator.getGamepads() : (navigator.webkitGetGamepads ? navigator.webkitGetGamepads() : null)))
        ? 0 : -1;
    }

  
  
  
  function registerBeforeUnloadEventCallback(target, userData, useCapture, callbackfunc, eventTypeId, eventTypeString) {
      var beforeUnloadEventHandlerFunc = function(e = event) {
        // Note: This is always called on the main browser thread, since it needs synchronously return a value!
        var confirmationMessage = getWasmTableEntry(callbackfunc)(eventTypeId, 0, userData);
        
        if (confirmationMessage) {
          confirmationMessage = UTF8ToString(confirmationMessage);
        }
        if (confirmationMessage) {
          e.preventDefault();
          e.returnValue = confirmationMessage;
          return confirmationMessage;
        }
      };
  
      var eventHandler = {
        target: findEventTarget(target),
        eventTypeString,
        callbackfunc,
        handlerFunc: beforeUnloadEventHandlerFunc,
        useCapture
      };
      return JSEvents.registerOrRemoveHandler(eventHandler);
    }
  function _emscripten_set_beforeunload_callback_on_thread(userData, callbackfunc, targetThread) {
      if (typeof onbeforeunload == 'undefined') return -1;
      // beforeunload callback can only be registered on the main browser thread, because the page will go away immediately after returning from the handler,
      // and there is no time to start proxying it anywhere.
      if (targetThread !== 1) return -5;
      return registerBeforeUnloadEventCallback(2, userData, true, callbackfunc, 28, "beforeunload");
    }

  
  
  
  
  function registerFocusEventCallback(target, userData, useCapture, callbackfunc, eventTypeId, eventTypeString, targetThread) {
      if (!JSEvents.focusEvent) JSEvents.focusEvent = _malloc( 256 );
  
      var focusEventHandlerFunc = function(e = event) {
        var nodeName = JSEvents.getNodeNameForTarget(e.target);
        var id = e.target.id ? e.target.id : '';
  
        var focusEvent = JSEvents.focusEvent;
        stringToUTF8(nodeName, focusEvent + 0, 128);
        stringToUTF8(id, focusEvent + 128, 128);
  
        if (getWasmTableEntry(callbackfunc)(eventTypeId, focusEvent, userData)) e.preventDefault();
      };
  
      var eventHandler = {
        target: findEventTarget(target),
        eventTypeString,
        callbackfunc,
        handlerFunc: focusEventHandlerFunc,
        useCapture
      };
      return JSEvents.registerOrRemoveHandler(eventHandler);
    }
  function _emscripten_set_blur_callback_on_thread(target, userData, useCapture, callbackfunc, targetThread) {
      return registerFocusEventCallback(target, userData, useCapture, callbackfunc, 12, "blur", targetThread);
    }


  
  function _emscripten_set_element_css_size(target, width, height) {
      target = findEventTarget(target);
      if (!target) return -4;
  
      target.style.width = width + "px";
      target.style.height = height + "px";
  
      return 0;
    }

  function _emscripten_set_focus_callback_on_thread(target, userData, useCapture, callbackfunc, targetThread) {
      return registerFocusEventCallback(target, userData, useCapture, callbackfunc, 13, "focus", targetThread);
    }

  
  
  
  function fillFullscreenChangeEventData(eventStruct) {
      var fullscreenElement = document.fullscreenElement || document.mozFullScreenElement || document.webkitFullscreenElement || document.msFullscreenElement;
      var isFullscreen = !!fullscreenElement;
      // Assigning a boolean to HEAP32 with expected type coercion.
      /** @suppress{checkTypes} */
      HEAP32[((eventStruct)>>2)] = isFullscreen;
      HEAP32[(((eventStruct)+(4))>>2)] = JSEvents.fullscreenEnabled();
      // If transitioning to fullscreen, report info about the element that is now fullscreen.
      // If transitioning to windowed mode, report info about the element that just was fullscreen.
      var reportedElement = isFullscreen ? fullscreenElement : JSEvents.previousFullscreenElement;
      var nodeName = JSEvents.getNodeNameForTarget(reportedElement);
      var id = (reportedElement && reportedElement.id) ? reportedElement.id : '';
      stringToUTF8(nodeName, eventStruct + 8, 128);
      stringToUTF8(id, eventStruct + 136, 128);
      HEAP32[(((eventStruct)+(264))>>2)] = reportedElement ? reportedElement.clientWidth : 0;
      HEAP32[(((eventStruct)+(268))>>2)] = reportedElement ? reportedElement.clientHeight : 0;
      HEAP32[(((eventStruct)+(272))>>2)] = screen.width;
      HEAP32[(((eventStruct)+(276))>>2)] = screen.height;
      if (isFullscreen) {
        JSEvents.previousFullscreenElement = fullscreenElement;
      }
    }
  
  
  
  function registerFullscreenChangeEventCallback(target, userData, useCapture, callbackfunc, eventTypeId, eventTypeString, targetThread) {
      if (!JSEvents.fullscreenChangeEvent) JSEvents.fullscreenChangeEvent = _malloc( 280 );
  
      var fullscreenChangeEventhandlerFunc = function(e = event) {
        var fullscreenChangeEvent = JSEvents.fullscreenChangeEvent;
  
        fillFullscreenChangeEventData(fullscreenChangeEvent);
  
        if (getWasmTableEntry(callbackfunc)(eventTypeId, fullscreenChangeEvent, userData)) e.preventDefault();
      };
  
      var eventHandler = {
        target,
        eventTypeString,
        callbackfunc,
        handlerFunc: fullscreenChangeEventhandlerFunc,
        useCapture
      };
      return JSEvents.registerOrRemoveHandler(eventHandler);
    }
  
  
  function _emscripten_set_fullscreenchange_callback_on_thread(target, userData, useCapture, callbackfunc, targetThread) {
      if (!JSEvents.fullscreenEnabled()) return -1;
      target = findEventTarget(target);
      if (!target) return -4;
  
      // Unprefixed Fullscreen API shipped in Chromium 71 (https://bugs.chromium.org/p/chromium/issues/detail?id=383813)
      // As of Safari 13.0.3 on macOS Catalina 10.15.1 still ships with prefixed webkitfullscreenchange. TODO: revisit this check once Safari ships unprefixed version.
      registerFullscreenChangeEventCallback(target, userData, useCapture, callbackfunc, 19, "webkitfullscreenchange", targetThread);
  
      return registerFullscreenChangeEventCallback(target, userData, useCapture, callbackfunc, 19, "fullscreenchange", targetThread);
    }

  
  
  
  
  function registerGamepadEventCallback(target, userData, useCapture, callbackfunc, eventTypeId, eventTypeString, targetThread) {
      if (!JSEvents.gamepadEvent) JSEvents.gamepadEvent = _malloc( 1432 );
  
      var gamepadEventHandlerFunc = function(e = event) {
        var gamepadEvent = JSEvents.gamepadEvent;
        fillGamepadEventData(gamepadEvent, e["gamepad"]);
  
        if (getWasmTableEntry(callbackfunc)(eventTypeId, gamepadEvent, userData)) e.preventDefault();
      };
  
      var eventHandler = {
        target: findEventTarget(target),
        allowsDeferredCalls: true,
        eventTypeString,
        callbackfunc,
        handlerFunc: gamepadEventHandlerFunc,
        useCapture
      };
      return JSEvents.registerOrRemoveHandler(eventHandler);
    }
  function _emscripten_set_gamepadconnected_callback_on_thread(userData, useCapture, callbackfunc, targetThread) {
      if (!navigator.getGamepads && !navigator.webkitGetGamepads) return -1;
      return registerGamepadEventCallback(2, userData, useCapture, callbackfunc, 26, "gamepadconnected", targetThread);
    }

  function _emscripten_set_gamepaddisconnected_callback_on_thread(userData, useCapture, callbackfunc, targetThread) {
      if (!navigator.getGamepads && !navigator.webkitGetGamepads) return -1;
      return registerGamepadEventCallback(2, userData, useCapture, callbackfunc, 27, "gamepaddisconnected", targetThread);
    }

  
  
  
  
  function registerKeyEventCallback(target, userData, useCapture, callbackfunc, eventTypeId, eventTypeString, targetThread) {
      if (!JSEvents.keyEvent) JSEvents.keyEvent = _malloc( 176 );
  
      var keyEventHandlerFunc = function(e) {
        assert(e);
  
        var keyEventData = JSEvents.keyEvent;
        HEAPF64[((keyEventData)>>3)] = e.timeStamp;
  
        var idx = keyEventData >> 2;
  
        HEAP32[idx + 2] = e.location;
        HEAP32[idx + 3] = e.ctrlKey;
        HEAP32[idx + 4] = e.shiftKey;
        HEAP32[idx + 5] = e.altKey;
        HEAP32[idx + 6] = e.metaKey;
        HEAP32[idx + 7] = e.repeat;
        HEAP32[idx + 8] = e.charCode;
        HEAP32[idx + 9] = e.keyCode;
        HEAP32[idx + 10] = e.which;
        stringToUTF8(e.key || '', keyEventData + 44, 32);
        stringToUTF8(e.code || '', keyEventData + 76, 32);
        stringToUTF8(e.char || '', keyEventData + 108, 32);
        stringToUTF8(e.locale || '', keyEventData + 140, 32);
  
        if (getWasmTableEntry(callbackfunc)(eventTypeId, keyEventData, userData)) e.preventDefault();
      };
  
      var eventHandler = {
        target: findEventTarget(target),
        allowsDeferredCalls: true,
        eventTypeString,
        callbackfunc,
        handlerFunc: keyEventHandlerFunc,
        useCapture
      };
      return JSEvents.registerOrRemoveHandler(eventHandler);
    }
  function _emscripten_set_keydown_callback_on_thread(target, userData, useCapture, callbackfunc, targetThread) {
      return registerKeyEventCallback(target, userData, useCapture, callbackfunc, 2, "keydown", targetThread);
    }

  function _emscripten_set_keypress_callback_on_thread(target, userData, useCapture, callbackfunc, targetThread) {
      return registerKeyEventCallback(target, userData, useCapture, callbackfunc, 1, "keypress", targetThread);
    }

  function _emscripten_set_keyup_callback_on_thread(target, userData, useCapture, callbackfunc, targetThread) {
      return registerKeyEventCallback(target, userData, useCapture, callbackfunc, 3, "keyup", targetThread);
    }

  
  
  var _emscripten_set_main_loop_arg = function(func, arg, fps, simulateInfiniteLoop) {
      var browserIterationFunc = () => getWasmTableEntry(func)(arg);
      setMainLoop(browserIterationFunc, fps, simulateInfiniteLoop, arg);
    };

  
  
  
  function fillMouseEventData(eventStruct, e, target) {
      assert(eventStruct % 4 == 0);
      HEAPF64[((eventStruct)>>3)] = e.timeStamp;
      var idx = eventStruct >> 2;
      HEAP32[idx + 2] = e.screenX;
      HEAP32[idx + 3] = e.screenY;
      HEAP32[idx + 4] = e.clientX;
      HEAP32[idx + 5] = e.clientY;
      HEAP32[idx + 6] = e.ctrlKey;
      HEAP32[idx + 7] = e.shiftKey;
      HEAP32[idx + 8] = e.altKey;
      HEAP32[idx + 9] = e.metaKey;
      HEAP16[idx*2 + 20] = e.button;
      HEAP16[idx*2 + 21] = e.buttons;
  
      HEAP32[idx + 11] = e["movementX"]
        ;
  
      HEAP32[idx + 12] = e["movementY"]
        ;
  
      var rect = getBoundingClientRect(target);
      HEAP32[idx + 13] = e.clientX - rect.left;
      HEAP32[idx + 14] = e.clientY - rect.top;
  
    }
  
  
  
  function registerMouseEventCallback(target, userData, useCapture, callbackfunc, eventTypeId, eventTypeString, targetThread) {
      if (!JSEvents.mouseEvent) JSEvents.mouseEvent = _malloc( 72 );
      target = findEventTarget(target);
  
      var mouseEventHandlerFunc = function(e = event) {
        // TODO: Make this access thread safe, or this could update live while app is reading it.
        fillMouseEventData(JSEvents.mouseEvent, e, target);
  
        if (getWasmTableEntry(callbackfunc)(eventTypeId, JSEvents.mouseEvent, userData)) e.preventDefault();
      };
  
      var eventHandler = {
        target,
        allowsDeferredCalls: eventTypeString != 'mousemove' && eventTypeString != 'mouseenter' && eventTypeString != 'mouseleave', // Mouse move events do not allow fullscreen/pointer lock requests to be handled in them!
        eventTypeString,
        callbackfunc,
        handlerFunc: mouseEventHandlerFunc,
        useCapture
      };
      return JSEvents.registerOrRemoveHandler(eventHandler);
    }
  function _emscripten_set_mousedown_callback_on_thread(target, userData, useCapture, callbackfunc, targetThread) {
      return registerMouseEventCallback(target, userData, useCapture, callbackfunc, 5, "mousedown", targetThread);
    }

  function _emscripten_set_mouseenter_callback_on_thread(target, userData, useCapture, callbackfunc, targetThread) {
      return registerMouseEventCallback(target, userData, useCapture, callbackfunc, 33, "mouseenter", targetThread);
    }

  function _emscripten_set_mouseleave_callback_on_thread(target, userData, useCapture, callbackfunc, targetThread) {
      return registerMouseEventCallback(target, userData, useCapture, callbackfunc, 34, "mouseleave", targetThread);
    }

  function _emscripten_set_mousemove_callback_on_thread(target, userData, useCapture, callbackfunc, targetThread) {
      return registerMouseEventCallback(target, userData, useCapture, callbackfunc, 8, "mousemove", targetThread);
    }

  function _emscripten_set_mouseup_callback_on_thread(target, userData, useCapture, callbackfunc, targetThread) {
      return registerMouseEventCallback(target, userData, useCapture, callbackfunc, 6, "mouseup", targetThread);
    }

  
  
  
  function fillPointerlockChangeEventData(eventStruct) {
      var pointerLockElement = document.pointerLockElement || document.mozPointerLockElement || document.webkitPointerLockElement || document.msPointerLockElement;
      var isPointerlocked = !!pointerLockElement;
      // Assigning a boolean to HEAP32 with expected type coercion.
      /** @suppress{checkTypes} */
      HEAP32[((eventStruct)>>2)] = isPointerlocked;
      var nodeName = JSEvents.getNodeNameForTarget(pointerLockElement);
      var id = (pointerLockElement && pointerLockElement.id) ? pointerLockElement.id : '';
      stringToUTF8(nodeName, eventStruct + 4, 128);
      stringToUTF8(id, eventStruct + 132, 128);
    }
  
  
  
  function registerPointerlockChangeEventCallback(target, userData, useCapture, callbackfunc, eventTypeId, eventTypeString, targetThread) {
      if (!JSEvents.pointerlockChangeEvent) JSEvents.pointerlockChangeEvent = _malloc( 260 );
  
      var pointerlockChangeEventHandlerFunc = function(e = event) {
        var pointerlockChangeEvent = JSEvents.pointerlockChangeEvent;
        fillPointerlockChangeEventData(pointerlockChangeEvent);
  
        if (getWasmTableEntry(callbackfunc)(eventTypeId, pointerlockChangeEvent, userData)) e.preventDefault();
      };
  
      var eventHandler = {
        target,
        eventTypeString,
        callbackfunc,
        handlerFunc: pointerlockChangeEventHandlerFunc,
        useCapture
      };
      return JSEvents.registerOrRemoveHandler(eventHandler);
    }
  
  
  /** @suppress {missingProperties} */
  function _emscripten_set_pointerlockchange_callback_on_thread(target, userData, useCapture, callbackfunc, targetThread) {
      // TODO: Currently not supported in pthreads or in --proxy-to-worker mode. (In pthreads mode, document object is not defined)
      if (!document || !document.body || (!document.body.requestPointerLock && !document.body.mozRequestPointerLock && !document.body.webkitRequestPointerLock && !document.body.msRequestPointerLock)) {
        return -1;
      }
  
      target = findEventTarget(target);
      if (!target) return -4;
      registerPointerlockChangeEventCallback(target, userData, useCapture, callbackfunc, 20, "mozpointerlockchange", targetThread);
      registerPointerlockChangeEventCallback(target, userData, useCapture, callbackfunc, 20, "webkitpointerlockchange", targetThread);
      registerPointerlockChangeEventCallback(target, userData, useCapture, callbackfunc, 20, "mspointerlockchange", targetThread);
      return registerPointerlockChangeEventCallback(target, userData, useCapture, callbackfunc, 20, "pointerlockchange", targetThread);
    }

  
  
  
  function registerUiEventCallback(target, userData, useCapture, callbackfunc, eventTypeId, eventTypeString, targetThread) {
      if (!JSEvents.uiEvent) JSEvents.uiEvent = _malloc( 36 );
  
      target = findEventTarget(target);
  
      var uiEventHandlerFunc = function(e = event) {
        if (e.target != target) {
          // Never take ui events such as scroll via a 'bubbled' route, but always from the direct element that
          // was targeted. Otherwise e.g. if app logs a message in response to a page scroll, the Emscripten log
          // message box could cause to scroll, generating a new (bubbled) scroll message, causing a new log print,
          // causing a new scroll, etc..
          return;
        }
        var b = document.body; // Take document.body to a variable, Closure compiler does not outline access to it on its own.
        if (!b) {
          // During a page unload 'body' can be null, with "Cannot read property 'clientWidth' of null" being thrown
          return;
        }
        var uiEvent = JSEvents.uiEvent;
        HEAP32[((uiEvent)>>2)] = e.detail;
        HEAP32[(((uiEvent)+(4))>>2)] = b.clientWidth;
        HEAP32[(((uiEvent)+(8))>>2)] = b.clientHeight;
        HEAP32[(((uiEvent)+(12))>>2)] = innerWidth;
        HEAP32[(((uiEvent)+(16))>>2)] = innerHeight;
        HEAP32[(((uiEvent)+(20))>>2)] = outerWidth;
        HEAP32[(((uiEvent)+(24))>>2)] = outerHeight;
        HEAP32[(((uiEvent)+(28))>>2)] = pageXOffset;
        HEAP32[(((uiEvent)+(32))>>2)] = pageYOffset;
        if (getWasmTableEntry(callbackfunc)(eventTypeId, uiEvent, userData)) e.preventDefault();
      };
  
      var eventHandler = {
        target,
        eventTypeString,
        callbackfunc,
        handlerFunc: uiEventHandlerFunc,
        useCapture
      };
      return JSEvents.registerOrRemoveHandler(eventHandler);
    }
  function _emscripten_set_resize_callback_on_thread(target, userData, useCapture, callbackfunc, targetThread) {
      return registerUiEventCallback(target, userData, useCapture, callbackfunc, 10, "resize", targetThread);
    }

  
  
  
  
  function registerTouchEventCallback(target, userData, useCapture, callbackfunc, eventTypeId, eventTypeString, targetThread) {
      if (!JSEvents.touchEvent) JSEvents.touchEvent = _malloc( 1696 );
  
      target = findEventTarget(target);
  
      var touchEventHandlerFunc = function(e) {
        assert(e);
        var t, touches = {}, et = e.touches;
        // To ease marshalling different kinds of touches that browser reports (all touches are listed in e.touches, 
        // only changed touches in e.changedTouches, and touches on target at a.targetTouches), mark a boolean in
        // each Touch object so that we can later loop only once over all touches we see to marshall over to Wasm.
  
        for (var i = 0; i < et.length; ++i) {
          t = et[i];
          // Browser might recycle the generated Touch objects between each frame (Firefox on Android), so reset any
          // changed/target states we may have set from previous frame.
          t.isChanged = t.onTarget = 0;
          touches[t.identifier] = t;
        }
        // Mark which touches are part of the changedTouches list.
        for (var i = 0; i < e.changedTouches.length; ++i) {
          t = e.changedTouches[i];
          t.isChanged = 1;
          touches[t.identifier] = t;
        }
        // Mark which touches are part of the targetTouches list.
        for (var i = 0; i < e.targetTouches.length; ++i) {
          touches[e.targetTouches[i].identifier].onTarget = 1;
        }
  
        var touchEvent = JSEvents.touchEvent;
        HEAPF64[((touchEvent)>>3)] = e.timeStamp;
        var idx = touchEvent>>2; // Pre-shift the ptr to index to HEAP32 to save code size
        HEAP32[idx + 3] = e.ctrlKey;
        HEAP32[idx + 4] = e.shiftKey;
        HEAP32[idx + 5] = e.altKey;
        HEAP32[idx + 6] = e.metaKey;
        idx += 7; // Advance to the start of the touch array.
        var targetRect = getBoundingClientRect(target);
        var numTouches = 0;
        for (var i in touches) {
          t = touches[i];
          HEAP32[idx + 0] = t.identifier;
          HEAP32[idx + 1] = t.screenX;
          HEAP32[idx + 2] = t.screenY;
          HEAP32[idx + 3] = t.clientX;
          HEAP32[idx + 4] = t.clientY;
          HEAP32[idx + 5] = t.pageX;
          HEAP32[idx + 6] = t.pageY;
          HEAP32[idx + 7] = t.isChanged;
          HEAP32[idx + 8] = t.onTarget;
          HEAP32[idx + 9] = t.clientX - targetRect.left;
          HEAP32[idx + 10] = t.clientY - targetRect.top;
  
          idx += 13;
  
          if (++numTouches > 31) {
            break;
          }
        }
        HEAP32[(((touchEvent)+(8))>>2)] = numTouches;
  
        if (getWasmTableEntry(callbackfunc)(eventTypeId, touchEvent, userData)) e.preventDefault();
      };
  
      var eventHandler = {
        target,
        allowsDeferredCalls: eventTypeString == 'touchstart' || eventTypeString == 'touchend',
        eventTypeString,
        callbackfunc,
        handlerFunc: touchEventHandlerFunc,
        useCapture
      };
      return JSEvents.registerOrRemoveHandler(eventHandler);
    }
  function _emscripten_set_touchcancel_callback_on_thread(target, userData, useCapture, callbackfunc, targetThread) {
      return registerTouchEventCallback(target, userData, useCapture, callbackfunc, 25, "touchcancel", targetThread);
    }

  function _emscripten_set_touchend_callback_on_thread(target, userData, useCapture, callbackfunc, targetThread) {
      return registerTouchEventCallback(target, userData, useCapture, callbackfunc, 23, "touchend", targetThread);
    }

  function _emscripten_set_touchmove_callback_on_thread(target, userData, useCapture, callbackfunc, targetThread) {
      return registerTouchEventCallback(target, userData, useCapture, callbackfunc, 24, "touchmove", targetThread);
    }

  function _emscripten_set_touchstart_callback_on_thread(target, userData, useCapture, callbackfunc, targetThread) {
      return registerTouchEventCallback(target, userData, useCapture, callbackfunc, 22, "touchstart", targetThread);
    }

  
  function fillVisibilityChangeEventData(eventStruct) {
      var visibilityStates = [ "hidden", "visible", "prerender", "unloaded" ];
      var visibilityState = visibilityStates.indexOf(document.visibilityState);
  
      // Assigning a boolean to HEAP32 with expected type coercion.
      /** @suppress{checkTypes} */
      HEAP32[((eventStruct)>>2)] = document.hidden;
      HEAP32[(((eventStruct)+(4))>>2)] = visibilityState;
    }
  
  
  
  function registerVisibilityChangeEventCallback(target, userData, useCapture, callbackfunc, eventTypeId, eventTypeString, targetThread) {
      if (!JSEvents.visibilityChangeEvent) JSEvents.visibilityChangeEvent = _malloc( 8 );
  
      var visibilityChangeEventHandlerFunc = function(e = event) {
        var visibilityChangeEvent = JSEvents.visibilityChangeEvent;
  
        fillVisibilityChangeEventData(visibilityChangeEvent);
  
        if (getWasmTableEntry(callbackfunc)(eventTypeId, visibilityChangeEvent, userData)) e.preventDefault();
      };
  
      var eventHandler = {
        target,
        eventTypeString,
        callbackfunc,
        handlerFunc: visibilityChangeEventHandlerFunc,
        useCapture
      };
      return JSEvents.registerOrRemoveHandler(eventHandler);
    }
  
  function _emscripten_set_visibilitychange_callback_on_thread(userData, useCapture, callbackfunc, targetThread) {
    if (!specialHTMLTargets[1]) {
      return -4;
    }
      return registerVisibilityChangeEventCallback(specialHTMLTargets[1], userData, useCapture, callbackfunc, 21, "visibilitychange", targetThread);
    }

  
  
  
  
  
  function registerWheelEventCallback(target, userData, useCapture, callbackfunc, eventTypeId, eventTypeString, targetThread) {
      if (!JSEvents.wheelEvent) JSEvents.wheelEvent = _malloc( 104 );
  
      // The DOM Level 3 events spec event 'wheel'
      var wheelHandlerFunc = function(e = event) {
        var wheelEvent = JSEvents.wheelEvent;
        fillMouseEventData(wheelEvent, e, target);
        HEAPF64[(((wheelEvent)+(72))>>3)] = e["deltaX"];
        HEAPF64[(((wheelEvent)+(80))>>3)] = e["deltaY"];
        HEAPF64[(((wheelEvent)+(88))>>3)] = e["deltaZ"];
        HEAP32[(((wheelEvent)+(96))>>2)] = e["deltaMode"];
        if (getWasmTableEntry(callbackfunc)(eventTypeId, wheelEvent, userData)) e.preventDefault();
      };
  
      var eventHandler = {
        target,
        allowsDeferredCalls: true,
        eventTypeString,
        callbackfunc,
        handlerFunc: wheelHandlerFunc,
        useCapture
      };
      return JSEvents.registerOrRemoveHandler(eventHandler);
    }
  
  function _emscripten_set_wheel_callback_on_thread(target, userData, useCapture, callbackfunc, targetThread) {
      target = findEventTarget(target);
      if (!target) return -4;
      if (typeof target.onwheel != 'undefined') {
        return registerWheelEventCallback(target, userData, useCapture, callbackfunc, 9, "wheel", targetThread);
      } else {
        return -1;
      }
    }

  
  function _emscripten_set_window_title(title) {
      setWindowTitle(UTF8ToString(title));
    }

  function _emscripten_sleep() {
      throw 'Please compile your program with async support in order to use asynchronous operations like emscripten_sleep';
    }

  var ENV = {
  };
  
  var getExecutableName = () => {
      return thisProgram || './this.program';
    };
  var getEnvStrings = () => {
      if (!getEnvStrings.strings) {
        // Default values.
        // Browser language detection #8751
        var lang = ((typeof navigator == 'object' && navigator.languages && navigator.languages[0]) || 'C').replace('-', '_') + '.UTF-8';
        var env = {
          'USER': 'web_user',
          'LOGNAME': 'web_user',
          'PATH': '/',
          'PWD': '/',
          'HOME': '/home/web_user',
          'LANG': lang,
          '_': getExecutableName()
        };
        // Apply the user-provided values, if any.
        for (var x in ENV) {
          // x is a key in ENV; if ENV[x] is undefined, that means it was
          // explicitly set to be so. We allow user code to do that to
          // force variables with default values to remain unset.
          if (ENV[x] === undefined) delete env[x];
          else env[x] = ENV[x];
        }
        var strings = [];
        for (var x in env) {
          strings.push(`${x}=${env[x]}`);
        }
        getEnvStrings.strings = strings;
      }
      return getEnvStrings.strings;
    };
  
  var stringToAscii = (str, buffer) => {
      for (var i = 0; i < str.length; ++i) {
        assert(str.charCodeAt(i) === (str.charCodeAt(i) & 0xff));
        HEAP8[((buffer++)>>0)] = str.charCodeAt(i);
      }
      // Null-terminate the string
      HEAP8[((buffer)>>0)] = 0;
    };
  
  var _environ_get = (__environ, environ_buf) => {
      var bufSize = 0;
      getEnvStrings().forEach(function(string, i) {
        var ptr = environ_buf + bufSize;
        HEAPU32[(((__environ)+(i*4))>>2)] = ptr;
        stringToAscii(string, ptr);
        bufSize += string.length + 1;
      });
      return 0;
    };

  
  var _environ_sizes_get = (penviron_count, penviron_buf_size) => {
      var strings = getEnvStrings();
      HEAPU32[((penviron_count)>>2)] = strings.length;
      var bufSize = 0;
      strings.forEach(function(string) {
        bufSize += string.length + 1;
      });
      HEAPU32[((penviron_buf_size)>>2)] = bufSize;
      return 0;
    };


  function _fd_close(fd) {
  try {
  
      var stream = SYSCALLS.getStreamFromFD(fd);
      FS.close(stream);
      return 0;
    } catch (e) {
    if (typeof FS == 'undefined' || !(e.name === 'ErrnoError')) throw e;
    return e.errno;
  }
  }

  /** @param {number=} offset */
  var doReadv = (stream, iov, iovcnt, offset) => {
      var ret = 0;
      for (var i = 0; i < iovcnt; i++) {
        var ptr = HEAPU32[((iov)>>2)];
        var len = HEAPU32[(((iov)+(4))>>2)];
        iov += 8;
        var curr = FS.read(stream, HEAP8,ptr, len, offset);
        if (curr < 0) return -1;
        ret += curr;
        if (curr < len) break; // nothing more to read
        if (typeof offset !== 'undefined') {
          offset += curr;
        }
      }
      return ret;
    };
  
  function _fd_read(fd, iov, iovcnt, pnum) {
  try {
  
      var stream = SYSCALLS.getStreamFromFD(fd);
      var num = doReadv(stream, iov, iovcnt);
      HEAPU32[((pnum)>>2)] = num;
      return 0;
    } catch (e) {
    if (typeof FS == 'undefined' || !(e.name === 'ErrnoError')) throw e;
    return e.errno;
  }
  }

  
  function convertI32PairToI53Checked(lo, hi) {
      assert(lo == (lo >>> 0) || lo == (lo|0)); // lo should either be a i32 or a u32
      assert(hi === (hi|0));                    // hi should be a i32
      return ((hi + 0x200000) >>> 0 < 0x400001 - !!lo) ? (lo >>> 0) + hi * 4294967296 : NaN;
    }
  function _fd_seek(fd,offset_low, offset_high,whence,newOffset) {
    var offset = convertI32PairToI53Checked(offset_low, offset_high);;
  
    
  try {
  
      if (isNaN(offset)) return 61;
      var stream = SYSCALLS.getStreamFromFD(fd);
      FS.llseek(stream, offset, whence);
      (tempI64 = [stream.position>>>0,(tempDouble=stream.position,(+(Math.abs(tempDouble))) >= 1.0 ? (tempDouble > 0.0 ? (+(Math.floor((tempDouble)/4294967296.0)))>>>0 : (~~((+(Math.ceil((tempDouble - +(((~~(tempDouble)))>>>0))/4294967296.0)))))>>>0) : 0)], HEAP32[((newOffset)>>2)] = tempI64[0],HEAP32[(((newOffset)+(4))>>2)] = tempI64[1]);
      if (stream.getdents && offset === 0 && whence === 0) stream.getdents = null; // reset readdir state
      return 0;
    } catch (e) {
    if (typeof FS == 'undefined' || !(e.name === 'ErrnoError')) throw e;
    return e.errno;
  }
  ;
  }

  /** @param {number=} offset */
  var doWritev = (stream, iov, iovcnt, offset) => {
      var ret = 0;
      for (var i = 0; i < iovcnt; i++) {
        var ptr = HEAPU32[((iov)>>2)];
        var len = HEAPU32[(((iov)+(4))>>2)];
        iov += 8;
        var curr = FS.write(stream, HEAP8,ptr, len, offset);
        if (curr < 0) return -1;
        ret += curr;
        if (typeof offset !== 'undefined') {
          offset += curr;
        }
      }
      return ret;
    };
  
  function _fd_write(fd, iov, iovcnt, pnum) {
  try {
  
      var stream = SYSCALLS.getStreamFromFD(fd);
      var num = doWritev(stream, iov, iovcnt);
      HEAPU32[((pnum)>>2)] = num;
      return 0;
    } catch (e) {
    if (typeof FS == 'undefined' || !(e.name === 'ErrnoError')) throw e;
    return e.errno;
  }
  }













































  var isLeapYear = (year) => {
        return year%4 === 0 && (year%100 !== 0 || year%400 === 0);
    };
  
  var arraySum = (array, index) => {
      var sum = 0;
      for (var i = 0; i <= index; sum += array[i++]) {
        // no-op
      }
      return sum;
    };
  
  
  var MONTH_DAYS_LEAP = [31,29,31,30,31,30,31,31,30,31,30,31];
  
  var MONTH_DAYS_REGULAR = [31,28,31,30,31,30,31,31,30,31,30,31];
  var addDays = (date, days) => {
      var newDate = new Date(date.getTime());
      while (days > 0) {
        var leap = isLeapYear(newDate.getFullYear());
        var currentMonth = newDate.getMonth();
        var daysInCurrentMonth = (leap ? MONTH_DAYS_LEAP : MONTH_DAYS_REGULAR)[currentMonth];
  
        if (days > daysInCurrentMonth-newDate.getDate()) {
          // we spill over to next month
          days -= (daysInCurrentMonth-newDate.getDate()+1);
          newDate.setDate(1);
          if (currentMonth < 11) {
            newDate.setMonth(currentMonth+1)
          } else {
            newDate.setMonth(0);
            newDate.setFullYear(newDate.getFullYear()+1);
          }
        } else {
          // we stay in current month
          newDate.setDate(newDate.getDate()+days);
          return newDate;
        }
      }
  
      return newDate;
    };
  
  
  
  
  var writeArrayToMemory = (array, buffer) => {
      assert(array.length >= 0, 'writeArrayToMemory array must have a length (should be an array or typed array)')
      HEAP8.set(array, buffer);
    };
  
  var _strftime = (s, maxsize, format, tm) => {
      // size_t strftime(char *restrict s, size_t maxsize, const char *restrict format, const struct tm *restrict timeptr);
      // http://pubs.opengroup.org/onlinepubs/009695399/functions/strftime.html
  
      var tm_zone = HEAP32[(((tm)+(40))>>2)];
  
      var date = {
        tm_sec: HEAP32[((tm)>>2)],
        tm_min: HEAP32[(((tm)+(4))>>2)],
        tm_hour: HEAP32[(((tm)+(8))>>2)],
        tm_mday: HEAP32[(((tm)+(12))>>2)],
        tm_mon: HEAP32[(((tm)+(16))>>2)],
        tm_year: HEAP32[(((tm)+(20))>>2)],
        tm_wday: HEAP32[(((tm)+(24))>>2)],
        tm_yday: HEAP32[(((tm)+(28))>>2)],
        tm_isdst: HEAP32[(((tm)+(32))>>2)],
        tm_gmtoff: HEAP32[(((tm)+(36))>>2)],
        tm_zone: tm_zone ? UTF8ToString(tm_zone) : ''
      };
  
      var pattern = UTF8ToString(format);
  
      // expand format
      var EXPANSION_RULES_1 = {
        '%c': '%a %b %d %H:%M:%S %Y',     // Replaced by the locale's appropriate date and time representation - e.g., Mon Aug  3 14:02:01 2013
        '%D': '%m/%d/%y',                 // Equivalent to %m / %d / %y
        '%F': '%Y-%m-%d',                 // Equivalent to %Y - %m - %d
        '%h': '%b',                       // Equivalent to %b
        '%r': '%I:%M:%S %p',              // Replaced by the time in a.m. and p.m. notation
        '%R': '%H:%M',                    // Replaced by the time in 24-hour notation
        '%T': '%H:%M:%S',                 // Replaced by the time
        '%x': '%m/%d/%y',                 // Replaced by the locale's appropriate date representation
        '%X': '%H:%M:%S',                 // Replaced by the locale's appropriate time representation
        // Modified Conversion Specifiers
        '%Ec': '%c',                      // Replaced by the locale's alternative appropriate date and time representation.
        '%EC': '%C',                      // Replaced by the name of the base year (period) in the locale's alternative representation.
        '%Ex': '%m/%d/%y',                // Replaced by the locale's alternative date representation.
        '%EX': '%H:%M:%S',                // Replaced by the locale's alternative time representation.
        '%Ey': '%y',                      // Replaced by the offset from %EC (year only) in the locale's alternative representation.
        '%EY': '%Y',                      // Replaced by the full alternative year representation.
        '%Od': '%d',                      // Replaced by the day of the month, using the locale's alternative numeric symbols, filled as needed with leading zeros if there is any alternative symbol for zero; otherwise, with leading <space> characters.
        '%Oe': '%e',                      // Replaced by the day of the month, using the locale's alternative numeric symbols, filled as needed with leading <space> characters.
        '%OH': '%H',                      // Replaced by the hour (24-hour clock) using the locale's alternative numeric symbols.
        '%OI': '%I',                      // Replaced by the hour (12-hour clock) using the locale's alternative numeric symbols.
        '%Om': '%m',                      // Replaced by the month using the locale's alternative numeric symbols.
        '%OM': '%M',                      // Replaced by the minutes using the locale's alternative numeric symbols.
        '%OS': '%S',                      // Replaced by the seconds using the locale's alternative numeric symbols.
        '%Ou': '%u',                      // Replaced by the weekday as a number in the locale's alternative representation (Monday=1).
        '%OU': '%U',                      // Replaced by the week number of the year (Sunday as the first day of the week, rules corresponding to %U ) using the locale's alternative numeric symbols.
        '%OV': '%V',                      // Replaced by the week number of the year (Monday as the first day of the week, rules corresponding to %V ) using the locale's alternative numeric symbols.
        '%Ow': '%w',                      // Replaced by the number of the weekday (Sunday=0) using the locale's alternative numeric symbols.
        '%OW': '%W',                      // Replaced by the week number of the year (Monday as the first day of the week) using the locale's alternative numeric symbols.
        '%Oy': '%y',                      // Replaced by the year (offset from %C ) using the locale's alternative numeric symbols.
      };
      for (var rule in EXPANSION_RULES_1) {
        pattern = pattern.replace(new RegExp(rule, 'g'), EXPANSION_RULES_1[rule]);
      }
  
      var WEEKDAYS = ['Sunday', 'Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday'];
      var MONTHS = ['January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December'];
  
      function leadingSomething(value, digits, character) {
        var str = typeof value == 'number' ? value.toString() : (value || '');
        while (str.length < digits) {
          str = character[0]+str;
        }
        return str;
      }
  
      function leadingNulls(value, digits) {
        return leadingSomething(value, digits, '0');
      }
  
      function compareByDay(date1, date2) {
        function sgn(value) {
          return value < 0 ? -1 : (value > 0 ? 1 : 0);
        }
  
        var compare;
        if ((compare = sgn(date1.getFullYear()-date2.getFullYear())) === 0) {
          if ((compare = sgn(date1.getMonth()-date2.getMonth())) === 0) {
            compare = sgn(date1.getDate()-date2.getDate());
          }
        }
        return compare;
      }
  
      function getFirstWeekStartDate(janFourth) {
          switch (janFourth.getDay()) {
            case 0: // Sunday
              return new Date(janFourth.getFullYear()-1, 11, 29);
            case 1: // Monday
              return janFourth;
            case 2: // Tuesday
              return new Date(janFourth.getFullYear(), 0, 3);
            case 3: // Wednesday
              return new Date(janFourth.getFullYear(), 0, 2);
            case 4: // Thursday
              return new Date(janFourth.getFullYear(), 0, 1);
            case 5: // Friday
              return new Date(janFourth.getFullYear()-1, 11, 31);
            case 6: // Saturday
              return new Date(janFourth.getFullYear()-1, 11, 30);
          }
      }
  
      function getWeekBasedYear(date) {
          var thisDate = addDays(new Date(date.tm_year+1900, 0, 1), date.tm_yday);
  
          var janFourthThisYear = new Date(thisDate.getFullYear(), 0, 4);
          var janFourthNextYear = new Date(thisDate.getFullYear()+1, 0, 4);
  
          var firstWeekStartThisYear = getFirstWeekStartDate(janFourthThisYear);
          var firstWeekStartNextYear = getFirstWeekStartDate(janFourthNextYear);
  
          if (compareByDay(firstWeekStartThisYear, thisDate) <= 0) {
            // this date is after the start of the first week of this year
            if (compareByDay(firstWeekStartNextYear, thisDate) <= 0) {
              return thisDate.getFullYear()+1;
            }
            return thisDate.getFullYear();
          }
          return thisDate.getFullYear()-1;
      }
  
      var EXPANSION_RULES_2 = {
        '%a': (date) => WEEKDAYS[date.tm_wday].substring(0,3) ,
        '%A': (date) => WEEKDAYS[date.tm_wday],
        '%b': (date) => MONTHS[date.tm_mon].substring(0,3),
        '%B': (date) => MONTHS[date.tm_mon],
        '%C': (date) => {
          var year = date.tm_year+1900;
          return leadingNulls((year/100)|0,2);
        },
        '%d': (date) => leadingNulls(date.tm_mday, 2),
        '%e': (date) => leadingSomething(date.tm_mday, 2, ' '),
        '%g': (date) => {
          // %g, %G, and %V give values according to the ISO 8601:2000 standard week-based year.
          // In this system, weeks begin on a Monday and week 1 of the year is the week that includes
          // January 4th, which is also the week that includes the first Thursday of the year, and
          // is also the first week that contains at least four days in the year.
          // If the first Monday of January is the 2nd, 3rd, or 4th, the preceding days are part of
          // the last week of the preceding year; thus, for Saturday 2nd January 1999,
          // %G is replaced by 1998 and %V is replaced by 53. If December 29th, 30th,
          // or 31st is a Monday, it and any following days are part of week 1 of the following year.
          // Thus, for Tuesday 30th December 1997, %G is replaced by 1998 and %V is replaced by 01.
  
          return getWeekBasedYear(date).toString().substring(2);
        },
        '%G': (date) => getWeekBasedYear(date),
        '%H': (date) => leadingNulls(date.tm_hour, 2),
        '%I': (date) => {
          var twelveHour = date.tm_hour;
          if (twelveHour == 0) twelveHour = 12;
          else if (twelveHour > 12) twelveHour -= 12;
          return leadingNulls(twelveHour, 2);
        },
        '%j': (date) => {
          // Day of the year (001-366)
          return leadingNulls(date.tm_mday + arraySum(isLeapYear(date.tm_year+1900) ? MONTH_DAYS_LEAP : MONTH_DAYS_REGULAR, date.tm_mon-1), 3);
        },
        '%m': (date) => leadingNulls(date.tm_mon+1, 2),
        '%M': (date) => leadingNulls(date.tm_min, 2),
        '%n': () => '\n',
        '%p': (date) => {
          if (date.tm_hour >= 0 && date.tm_hour < 12) {
            return 'AM';
          }
          return 'PM';
        },
        '%S': (date) => leadingNulls(date.tm_sec, 2),
        '%t': () => '\t',
        '%u': (date) => date.tm_wday || 7,
        '%U': (date) => {
          var days = date.tm_yday + 7 - date.tm_wday;
          return leadingNulls(Math.floor(days / 7), 2);
        },
        '%V': (date) => {
          // Replaced by the week number of the year (Monday as the first day of the week)
          // as a decimal number [01,53]. If the week containing 1 January has four
          // or more days in the new year, then it is considered week 1.
          // Otherwise, it is the last week of the previous year, and the next week is week 1.
          // Both January 4th and the first Thursday of January are always in week 1. [ tm_year, tm_wday, tm_yday]
          var val = Math.floor((date.tm_yday + 7 - (date.tm_wday + 6) % 7 ) / 7);
          // If 1 Jan is just 1-3 days past Monday, the previous week
          // is also in this year.
          if ((date.tm_wday + 371 - date.tm_yday - 2) % 7 <= 2) {
            val++;
          }
          if (!val) {
            val = 52;
            // If 31 December of prev year a Thursday, or Friday of a
            // leap year, then the prev year has 53 weeks.
            var dec31 = (date.tm_wday + 7 - date.tm_yday - 1) % 7;
            if (dec31 == 4 || (dec31 == 5 && isLeapYear(date.tm_year%400-1))) {
              val++;
            }
          } else if (val == 53) {
            // If 1 January is not a Thursday, and not a Wednesday of a
            // leap year, then this year has only 52 weeks.
            var jan1 = (date.tm_wday + 371 - date.tm_yday) % 7;
            if (jan1 != 4 && (jan1 != 3 || !isLeapYear(date.tm_year)))
              val = 1;
          }
          return leadingNulls(val, 2);
        },
        '%w': (date) => date.tm_wday,
        '%W': (date) => {
          var days = date.tm_yday + 7 - ((date.tm_wday + 6) % 7);
          return leadingNulls(Math.floor(days / 7), 2);
        },
        '%y': (date) => {
          // Replaced by the last two digits of the year as a decimal number [00,99]. [ tm_year]
          return (date.tm_year+1900).toString().substring(2);
        },
        // Replaced by the year as a decimal number (for example, 1997). [ tm_year]
        '%Y': (date) => date.tm_year+1900,
        '%z': (date) => {
          // Replaced by the offset from UTC in the ISO 8601:2000 standard format ( +hhmm or -hhmm ).
          // For example, "-0430" means 4 hours 30 minutes behind UTC (west of Greenwich).
          var off = date.tm_gmtoff;
          var ahead = off >= 0;
          off = Math.abs(off) / 60;
          // convert from minutes into hhmm format (which means 60 minutes = 100 units)
          off = (off / 60)*100 + (off % 60);
          return (ahead ? '+' : '-') + String("0000" + off).slice(-4);
        },
        '%Z': (date) => date.tm_zone,
        '%%': () => '%'
      };
  
      // Replace %% with a pair of NULLs (which cannot occur in a C string), then
      // re-inject them after processing.
      pattern = pattern.replace(/%%/g, '\0\0')
      for (var rule in EXPANSION_RULES_2) {
        if (pattern.includes(rule)) {
          pattern = pattern.replace(new RegExp(rule, 'g'), EXPANSION_RULES_2[rule](date));
        }
      }
      pattern = pattern.replace(/\0\0/g, '%')
  
      var bytes = intArrayFromString(pattern, false);
      if (bytes.length > maxsize) {
        return 0;
      }
  
      writeArrayToMemory(bytes, s);
      return bytes.length-1;
    };
  var _strftime_l = (s, maxsize, format, tm, loc) => {
      return _strftime(s, maxsize, format, tm); // no locale support yet
    };







  var FSNode = /** @constructor */ function(parent, name, mode, rdev) {
    if (!parent) {
      parent = this;  // root node sets parent to itself
    }
    this.parent = parent;
    this.mount = parent.mount;
    this.mounted = null;
    this.id = FS.nextInode++;
    this.name = name;
    this.mode = mode;
    this.node_ops = {};
    this.stream_ops = {};
    this.rdev = rdev;
  };
  var readMode = 292/*292*/ | 73/*73*/;
  var writeMode = 146/*146*/;
  Object.defineProperties(FSNode.prototype, {
   read: {
    get: /** @this{FSNode} */function() {
     return (this.mode & readMode) === readMode;
    },
    set: /** @this{FSNode} */function(val) {
     val ? this.mode |= readMode : this.mode &= ~readMode;
    }
   },
   write: {
    get: /** @this{FSNode} */function() {
     return (this.mode & writeMode) === writeMode;
    },
    set: /** @this{FSNode} */function(val) {
     val ? this.mode |= writeMode : this.mode &= ~writeMode;
    }
   },
   isFolder: {
    get: /** @this{FSNode} */function() {
     return FS.isDir(this.mode);
    }
   },
   isDevice: {
    get: /** @this{FSNode} */function() {
     return FS.isChrdev(this.mode);
    }
   }
  });
  FS.FSNode = FSNode;
  FS.createPreloadedFile = FS_createPreloadedFile;
  FS.staticInit();Module["FS_createPath"] = FS.createPath;Module["FS_createDataFile"] = FS.createDataFile;Module["FS_createPreloadedFile"] = FS.createPreloadedFile;Module["FS_unlink"] = FS.unlink;Module["FS_createLazyFile"] = FS.createLazyFile;Module["FS_createDevice"] = FS.createDevice;;
ERRNO_CODES = {
      'EPERM': 63,
      'ENOENT': 44,
      'ESRCH': 71,
      'EINTR': 27,
      'EIO': 29,
      'ENXIO': 60,
      'E2BIG': 1,
      'ENOEXEC': 45,
      'EBADF': 8,
      'ECHILD': 12,
      'EAGAIN': 6,
      'EWOULDBLOCK': 6,
      'ENOMEM': 48,
      'EACCES': 2,
      'EFAULT': 21,
      'ENOTBLK': 105,
      'EBUSY': 10,
      'EEXIST': 20,
      'EXDEV': 75,
      'ENODEV': 43,
      'ENOTDIR': 54,
      'EISDIR': 31,
      'EINVAL': 28,
      'ENFILE': 41,
      'EMFILE': 33,
      'ENOTTY': 59,
      'ETXTBSY': 74,
      'EFBIG': 22,
      'ENOSPC': 51,
      'ESPIPE': 70,
      'EROFS': 69,
      'EMLINK': 34,
      'EPIPE': 64,
      'EDOM': 18,
      'ERANGE': 68,
      'ENOMSG': 49,
      'EIDRM': 24,
      'ECHRNG': 106,
      'EL2NSYNC': 156,
      'EL3HLT': 107,
      'EL3RST': 108,
      'ELNRNG': 109,
      'EUNATCH': 110,
      'ENOCSI': 111,
      'EL2HLT': 112,
      'EDEADLK': 16,
      'ENOLCK': 46,
      'EBADE': 113,
      'EBADR': 114,
      'EXFULL': 115,
      'ENOANO': 104,
      'EBADRQC': 103,
      'EBADSLT': 102,
      'EDEADLOCK': 16,
      'EBFONT': 101,
      'ENOSTR': 100,
      'ENODATA': 116,
      'ETIME': 117,
      'ENOSR': 118,
      'ENONET': 119,
      'ENOPKG': 120,
      'EREMOTE': 121,
      'ENOLINK': 47,
      'EADV': 122,
      'ESRMNT': 123,
      'ECOMM': 124,
      'EPROTO': 65,
      'EMULTIHOP': 36,
      'EDOTDOT': 125,
      'EBADMSG': 9,
      'ENOTUNIQ': 126,
      'EBADFD': 127,
      'EREMCHG': 128,
      'ELIBACC': 129,
      'ELIBBAD': 130,
      'ELIBSCN': 131,
      'ELIBMAX': 132,
      'ELIBEXEC': 133,
      'ENOSYS': 52,
      'ENOTEMPTY': 55,
      'ENAMETOOLONG': 37,
      'ELOOP': 32,
      'EOPNOTSUPP': 138,
      'EPFNOSUPPORT': 139,
      'ECONNRESET': 15,
      'ENOBUFS': 42,
      'EAFNOSUPPORT': 5,
      'EPROTOTYPE': 67,
      'ENOTSOCK': 57,
      'ENOPROTOOPT': 50,
      'ESHUTDOWN': 140,
      'ECONNREFUSED': 14,
      'EADDRINUSE': 3,
      'ECONNABORTED': 13,
      'ENETUNREACH': 40,
      'ENETDOWN': 38,
      'ETIMEDOUT': 73,
      'EHOSTDOWN': 142,
      'EHOSTUNREACH': 23,
      'EINPROGRESS': 26,
      'EALREADY': 7,
      'EDESTADDRREQ': 17,
      'EMSGSIZE': 35,
      'EPROTONOSUPPORT': 66,
      'ESOCKTNOSUPPORT': 137,
      'EADDRNOTAVAIL': 4,
      'ENETRESET': 39,
      'EISCONN': 30,
      'ENOTCONN': 53,
      'ETOOMANYREFS': 141,
      'EUSERS': 136,
      'EDQUOT': 19,
      'ESTALE': 72,
      'ENOTSUP': 138,
      'ENOMEDIUM': 148,
      'EILSEQ': 25,
      'EOVERFLOW': 61,
      'ECANCELED': 11,
      'ENOTRECOVERABLE': 56,
      'EOWNERDEAD': 62,
      'ESTRPIPE': 135,
    };;

      // exports
      Module["requestFullscreen"] = function Module_requestFullscreen(lockPointer, resizeCanvas) { Browser.requestFullscreen(lockPointer, resizeCanvas) };
      Module["requestFullScreen"] = function Module_requestFullScreen() { Browser.requestFullScreen() };
      Module["requestAnimationFrame"] = function Module_requestAnimationFrame(func) { Browser.requestAnimationFrame(func) };
      Module["setCanvasSize"] = function Module_setCanvasSize(width, height, noUpdates) { Browser.setCanvasSize(width, height, noUpdates) };
      Module["pauseMainLoop"] = function Module_pauseMainLoop() { Browser.mainLoop.pause() };
      Module["resumeMainLoop"] = function Module_resumeMainLoop() { Browser.mainLoop.resume() };
      Module["getUserMedia"] = function Module_getUserMedia() { Browser.getUserMedia() };
      Module["createContext"] = function Module_createContext(canvas, useWebGL, setInModule, webGLContextAttributes) { return Browser.createContext(canvas, useWebGL, setInModule, webGLContextAttributes) };
      var preloadedImages = {};
      var preloadedAudios = {};;
var GLctx;;
for (var i = 0; i < 32; ++i) tempFixedLengthArray.push(new Array(i));;
var miniTempWebGLFloatBuffersStorage = new Float32Array(288);
  for (/**@suppress{duplicate}*/var i = 0; i < 288; ++i) {
  miniTempWebGLFloatBuffers[i] = miniTempWebGLFloatBuffersStorage.subarray(0, i+1);
  }
  ;
var miniTempWebGLIntBuffersStorage = new Int32Array(288);
  for (/**@suppress{duplicate}*/var i = 0; i < 288; ++i) {
  miniTempWebGLIntBuffers[i] = miniTempWebGLIntBuffersStorage.subarray(0, i+1);
  }
  ;
function checkIncomingModuleAPI() {
  ignoredModuleProp('fetchSettings');
}
var wasmImports = {
  __cxa_throw: ___cxa_throw,
  __syscall_fcntl64: ___syscall_fcntl64,
  __syscall_fstat64: ___syscall_fstat64,
  __syscall_getdents64: ___syscall_getdents64,
  __syscall_ioctl: ___syscall_ioctl,
  __syscall_lstat64: ___syscall_lstat64,
  __syscall_newfstatat: ___syscall_newfstatat,
  __syscall_openat: ___syscall_openat,
  __syscall_stat64: ___syscall_stat64,
  _emscripten_get_now_is_monotonic: __emscripten_get_now_is_monotonic,
  _emscripten_throw_longjmp: __emscripten_throw_longjmp,
  abort: _abort,
  eglBindAPI: _eglBindAPI,
  eglChooseConfig: _eglChooseConfig,
  eglCreateContext: _eglCreateContext,
  eglCreateWindowSurface: _eglCreateWindowSurface,
  eglDestroyContext: _eglDestroyContext,
  eglDestroySurface: _eglDestroySurface,
  eglGetConfigAttrib: _eglGetConfigAttrib,
  eglGetDisplay: _eglGetDisplay,
  eglGetError: _eglGetError,
  eglInitialize: _eglInitialize,
  eglMakeCurrent: _eglMakeCurrent,
  eglQueryString: _eglQueryString,
  eglSwapBuffers: _eglSwapBuffers,
  eglSwapInterval: _eglSwapInterval,
  eglTerminate: _eglTerminate,
  eglWaitGL: _eglWaitGL,
  eglWaitNative: _eglWaitNative,
  emscripten_asm_const_int: _emscripten_asm_const_int,
  emscripten_asm_const_int_sync_on_main_thread: _emscripten_asm_const_int_sync_on_main_thread,
  emscripten_cancel_main_loop: _emscripten_cancel_main_loop,
  emscripten_console_error: _emscripten_console_error,
  emscripten_date_now: _emscripten_date_now,
  emscripten_exit_fullscreen: _emscripten_exit_fullscreen,
  emscripten_exit_pointerlock: _emscripten_exit_pointerlock,
  emscripten_get_device_pixel_ratio: _emscripten_get_device_pixel_ratio,
  emscripten_get_element_css_size: _emscripten_get_element_css_size,
  emscripten_get_gamepad_status: _emscripten_get_gamepad_status,
  emscripten_get_heap_max: _emscripten_get_heap_max,
  emscripten_get_now: _emscripten_get_now,
  emscripten_get_num_gamepads: _emscripten_get_num_gamepads,
  emscripten_get_preloaded_image_data: _emscripten_get_preloaded_image_data,
  emscripten_get_preloaded_image_data_from_FILE: _emscripten_get_preloaded_image_data_from_FILE,
  emscripten_get_screen_size: _emscripten_get_screen_size,
  emscripten_glActiveTexture: _emscripten_glActiveTexture,
  emscripten_glAttachShader: _emscripten_glAttachShader,
  emscripten_glBeginQuery: _emscripten_glBeginQuery,
  emscripten_glBeginQueryEXT: _emscripten_glBeginQueryEXT,
  emscripten_glBeginTransformFeedback: _emscripten_glBeginTransformFeedback,
  emscripten_glBindAttribLocation: _emscripten_glBindAttribLocation,
  emscripten_glBindBuffer: _emscripten_glBindBuffer,
  emscripten_glBindBufferBase: _emscripten_glBindBufferBase,
  emscripten_glBindBufferRange: _emscripten_glBindBufferRange,
  emscripten_glBindFramebuffer: _emscripten_glBindFramebuffer,
  emscripten_glBindRenderbuffer: _emscripten_glBindRenderbuffer,
  emscripten_glBindSampler: _emscripten_glBindSampler,
  emscripten_glBindTexture: _emscripten_glBindTexture,
  emscripten_glBindTransformFeedback: _emscripten_glBindTransformFeedback,
  emscripten_glBindVertexArray: _emscripten_glBindVertexArray,
  emscripten_glBindVertexArrayOES: _emscripten_glBindVertexArrayOES,
  emscripten_glBlendColor: _emscripten_glBlendColor,
  emscripten_glBlendEquation: _emscripten_glBlendEquation,
  emscripten_glBlendEquationSeparate: _emscripten_glBlendEquationSeparate,
  emscripten_glBlendFunc: _emscripten_glBlendFunc,
  emscripten_glBlendFuncSeparate: _emscripten_glBlendFuncSeparate,
  emscripten_glBlitFramebuffer: _emscripten_glBlitFramebuffer,
  emscripten_glBufferData: _emscripten_glBufferData,
  emscripten_glBufferSubData: _emscripten_glBufferSubData,
  emscripten_glCheckFramebufferStatus: _emscripten_glCheckFramebufferStatus,
  emscripten_glClear: _emscripten_glClear,
  emscripten_glClearBufferfi: _emscripten_glClearBufferfi,
  emscripten_glClearBufferfv: _emscripten_glClearBufferfv,
  emscripten_glClearBufferiv: _emscripten_glClearBufferiv,
  emscripten_glClearBufferuiv: _emscripten_glClearBufferuiv,
  emscripten_glClearColor: _emscripten_glClearColor,
  emscripten_glClearDepthf: _emscripten_glClearDepthf,
  emscripten_glClearStencil: _emscripten_glClearStencil,
  emscripten_glClientWaitSync: _emscripten_glClientWaitSync,
  emscripten_glColorMask: _emscripten_glColorMask,
  emscripten_glCompileShader: _emscripten_glCompileShader,
  emscripten_glCompressedTexImage2D: _emscripten_glCompressedTexImage2D,
  emscripten_glCompressedTexImage3D: _emscripten_glCompressedTexImage3D,
  emscripten_glCompressedTexSubImage2D: _emscripten_glCompressedTexSubImage2D,
  emscripten_glCompressedTexSubImage3D: _emscripten_glCompressedTexSubImage3D,
  emscripten_glCopyBufferSubData: _emscripten_glCopyBufferSubData,
  emscripten_glCopyTexImage2D: _emscripten_glCopyTexImage2D,
  emscripten_glCopyTexSubImage2D: _emscripten_glCopyTexSubImage2D,
  emscripten_glCopyTexSubImage3D: _emscripten_glCopyTexSubImage3D,
  emscripten_glCreateProgram: _emscripten_glCreateProgram,
  emscripten_glCreateShader: _emscripten_glCreateShader,
  emscripten_glCullFace: _emscripten_glCullFace,
  emscripten_glDeleteBuffers: _emscripten_glDeleteBuffers,
  emscripten_glDeleteFramebuffers: _emscripten_glDeleteFramebuffers,
  emscripten_glDeleteProgram: _emscripten_glDeleteProgram,
  emscripten_glDeleteQueries: _emscripten_glDeleteQueries,
  emscripten_glDeleteQueriesEXT: _emscripten_glDeleteQueriesEXT,
  emscripten_glDeleteRenderbuffers: _emscripten_glDeleteRenderbuffers,
  emscripten_glDeleteSamplers: _emscripten_glDeleteSamplers,
  emscripten_glDeleteShader: _emscripten_glDeleteShader,
  emscripten_glDeleteSync: _emscripten_glDeleteSync,
  emscripten_glDeleteTextures: _emscripten_glDeleteTextures,
  emscripten_glDeleteTransformFeedbacks: _emscripten_glDeleteTransformFeedbacks,
  emscripten_glDeleteVertexArrays: _emscripten_glDeleteVertexArrays,
  emscripten_glDeleteVertexArraysOES: _emscripten_glDeleteVertexArraysOES,
  emscripten_glDepthFunc: _emscripten_glDepthFunc,
  emscripten_glDepthMask: _emscripten_glDepthMask,
  emscripten_glDepthRangef: _emscripten_glDepthRangef,
  emscripten_glDetachShader: _emscripten_glDetachShader,
  emscripten_glDisable: _emscripten_glDisable,
  emscripten_glDisableVertexAttribArray: _emscripten_glDisableVertexAttribArray,
  emscripten_glDrawArrays: _emscripten_glDrawArrays,
  emscripten_glDrawArraysInstanced: _emscripten_glDrawArraysInstanced,
  emscripten_glDrawArraysInstancedANGLE: _emscripten_glDrawArraysInstancedANGLE,
  emscripten_glDrawArraysInstancedARB: _emscripten_glDrawArraysInstancedARB,
  emscripten_glDrawArraysInstancedEXT: _emscripten_glDrawArraysInstancedEXT,
  emscripten_glDrawArraysInstancedNV: _emscripten_glDrawArraysInstancedNV,
  emscripten_glDrawBuffers: _emscripten_glDrawBuffers,
  emscripten_glDrawBuffersEXT: _emscripten_glDrawBuffersEXT,
  emscripten_glDrawBuffersWEBGL: _emscripten_glDrawBuffersWEBGL,
  emscripten_glDrawElements: _emscripten_glDrawElements,
  emscripten_glDrawElementsInstanced: _emscripten_glDrawElementsInstanced,
  emscripten_glDrawElementsInstancedANGLE: _emscripten_glDrawElementsInstancedANGLE,
  emscripten_glDrawElementsInstancedARB: _emscripten_glDrawElementsInstancedARB,
  emscripten_glDrawElementsInstancedEXT: _emscripten_glDrawElementsInstancedEXT,
  emscripten_glDrawElementsInstancedNV: _emscripten_glDrawElementsInstancedNV,
  emscripten_glDrawRangeElements: _emscripten_glDrawRangeElements,
  emscripten_glEnable: _emscripten_glEnable,
  emscripten_glEnableVertexAttribArray: _emscripten_glEnableVertexAttribArray,
  emscripten_glEndQuery: _emscripten_glEndQuery,
  emscripten_glEndQueryEXT: _emscripten_glEndQueryEXT,
  emscripten_glEndTransformFeedback: _emscripten_glEndTransformFeedback,
  emscripten_glFenceSync: _emscripten_glFenceSync,
  emscripten_glFinish: _emscripten_glFinish,
  emscripten_glFlush: _emscripten_glFlush,
  emscripten_glFlushMappedBufferRange: _emscripten_glFlushMappedBufferRange,
  emscripten_glFramebufferRenderbuffer: _emscripten_glFramebufferRenderbuffer,
  emscripten_glFramebufferTexture2D: _emscripten_glFramebufferTexture2D,
  emscripten_glFramebufferTextureLayer: _emscripten_glFramebufferTextureLayer,
  emscripten_glFrontFace: _emscripten_glFrontFace,
  emscripten_glGenBuffers: _emscripten_glGenBuffers,
  emscripten_glGenFramebuffers: _emscripten_glGenFramebuffers,
  emscripten_glGenQueries: _emscripten_glGenQueries,
  emscripten_glGenQueriesEXT: _emscripten_glGenQueriesEXT,
  emscripten_glGenRenderbuffers: _emscripten_glGenRenderbuffers,
  emscripten_glGenSamplers: _emscripten_glGenSamplers,
  emscripten_glGenTextures: _emscripten_glGenTextures,
  emscripten_glGenTransformFeedbacks: _emscripten_glGenTransformFeedbacks,
  emscripten_glGenVertexArrays: _emscripten_glGenVertexArrays,
  emscripten_glGenVertexArraysOES: _emscripten_glGenVertexArraysOES,
  emscripten_glGenerateMipmap: _emscripten_glGenerateMipmap,
  emscripten_glGetActiveAttrib: _emscripten_glGetActiveAttrib,
  emscripten_glGetActiveUniform: _emscripten_glGetActiveUniform,
  emscripten_glGetActiveUniformBlockName: _emscripten_glGetActiveUniformBlockName,
  emscripten_glGetActiveUniformBlockiv: _emscripten_glGetActiveUniformBlockiv,
  emscripten_glGetActiveUniformsiv: _emscripten_glGetActiveUniformsiv,
  emscripten_glGetAttachedShaders: _emscripten_glGetAttachedShaders,
  emscripten_glGetAttribLocation: _emscripten_glGetAttribLocation,
  emscripten_glGetBooleanv: _emscripten_glGetBooleanv,
  emscripten_glGetBufferParameteri64v: _emscripten_glGetBufferParameteri64v,
  emscripten_glGetBufferParameteriv: _emscripten_glGetBufferParameteriv,
  emscripten_glGetBufferPointerv: _emscripten_glGetBufferPointerv,
  emscripten_glGetError: _emscripten_glGetError,
  emscripten_glGetFloatv: _emscripten_glGetFloatv,
  emscripten_glGetFragDataLocation: _emscripten_glGetFragDataLocation,
  emscripten_glGetFramebufferAttachmentParameteriv: _emscripten_glGetFramebufferAttachmentParameteriv,
  emscripten_glGetInteger64i_v: _emscripten_glGetInteger64i_v,
  emscripten_glGetInteger64v: _emscripten_glGetInteger64v,
  emscripten_glGetIntegeri_v: _emscripten_glGetIntegeri_v,
  emscripten_glGetIntegerv: _emscripten_glGetIntegerv,
  emscripten_glGetInternalformativ: _emscripten_glGetInternalformativ,
  emscripten_glGetProgramBinary: _emscripten_glGetProgramBinary,
  emscripten_glGetProgramInfoLog: _emscripten_glGetProgramInfoLog,
  emscripten_glGetProgramiv: _emscripten_glGetProgramiv,
  emscripten_glGetQueryObjecti64vEXT: _emscripten_glGetQueryObjecti64vEXT,
  emscripten_glGetQueryObjectivEXT: _emscripten_glGetQueryObjectivEXT,
  emscripten_glGetQueryObjectui64vEXT: _emscripten_glGetQueryObjectui64vEXT,
  emscripten_glGetQueryObjectuiv: _emscripten_glGetQueryObjectuiv,
  emscripten_glGetQueryObjectuivEXT: _emscripten_glGetQueryObjectuivEXT,
  emscripten_glGetQueryiv: _emscripten_glGetQueryiv,
  emscripten_glGetQueryivEXT: _emscripten_glGetQueryivEXT,
  emscripten_glGetRenderbufferParameteriv: _emscripten_glGetRenderbufferParameteriv,
  emscripten_glGetSamplerParameterfv: _emscripten_glGetSamplerParameterfv,
  emscripten_glGetSamplerParameteriv: _emscripten_glGetSamplerParameteriv,
  emscripten_glGetShaderInfoLog: _emscripten_glGetShaderInfoLog,
  emscripten_glGetShaderPrecisionFormat: _emscripten_glGetShaderPrecisionFormat,
  emscripten_glGetShaderSource: _emscripten_glGetShaderSource,
  emscripten_glGetShaderiv: _emscripten_glGetShaderiv,
  emscripten_glGetString: _emscripten_glGetString,
  emscripten_glGetStringi: _emscripten_glGetStringi,
  emscripten_glGetSynciv: _emscripten_glGetSynciv,
  emscripten_glGetTexParameterfv: _emscripten_glGetTexParameterfv,
  emscripten_glGetTexParameteriv: _emscripten_glGetTexParameteriv,
  emscripten_glGetTransformFeedbackVarying: _emscripten_glGetTransformFeedbackVarying,
  emscripten_glGetUniformBlockIndex: _emscripten_glGetUniformBlockIndex,
  emscripten_glGetUniformIndices: _emscripten_glGetUniformIndices,
  emscripten_glGetUniformLocation: _emscripten_glGetUniformLocation,
  emscripten_glGetUniformfv: _emscripten_glGetUniformfv,
  emscripten_glGetUniformiv: _emscripten_glGetUniformiv,
  emscripten_glGetUniformuiv: _emscripten_glGetUniformuiv,
  emscripten_glGetVertexAttribIiv: _emscripten_glGetVertexAttribIiv,
  emscripten_glGetVertexAttribIuiv: _emscripten_glGetVertexAttribIuiv,
  emscripten_glGetVertexAttribPointerv: _emscripten_glGetVertexAttribPointerv,
  emscripten_glGetVertexAttribfv: _emscripten_glGetVertexAttribfv,
  emscripten_glGetVertexAttribiv: _emscripten_glGetVertexAttribiv,
  emscripten_glHint: _emscripten_glHint,
  emscripten_glInvalidateFramebuffer: _emscripten_glInvalidateFramebuffer,
  emscripten_glInvalidateSubFramebuffer: _emscripten_glInvalidateSubFramebuffer,
  emscripten_glIsBuffer: _emscripten_glIsBuffer,
  emscripten_glIsEnabled: _emscripten_glIsEnabled,
  emscripten_glIsFramebuffer: _emscripten_glIsFramebuffer,
  emscripten_glIsProgram: _emscripten_glIsProgram,
  emscripten_glIsQuery: _emscripten_glIsQuery,
  emscripten_glIsQueryEXT: _emscripten_glIsQueryEXT,
  emscripten_glIsRenderbuffer: _emscripten_glIsRenderbuffer,
  emscripten_glIsSampler: _emscripten_glIsSampler,
  emscripten_glIsShader: _emscripten_glIsShader,
  emscripten_glIsSync: _emscripten_glIsSync,
  emscripten_glIsTexture: _emscripten_glIsTexture,
  emscripten_glIsTransformFeedback: _emscripten_glIsTransformFeedback,
  emscripten_glIsVertexArray: _emscripten_glIsVertexArray,
  emscripten_glIsVertexArrayOES: _emscripten_glIsVertexArrayOES,
  emscripten_glLineWidth: _emscripten_glLineWidth,
  emscripten_glLinkProgram: _emscripten_glLinkProgram,
  emscripten_glMapBufferRange: _emscripten_glMapBufferRange,
  emscripten_glPauseTransformFeedback: _emscripten_glPauseTransformFeedback,
  emscripten_glPixelStorei: _emscripten_glPixelStorei,
  emscripten_glPolygonOffset: _emscripten_glPolygonOffset,
  emscripten_glProgramBinary: _emscripten_glProgramBinary,
  emscripten_glProgramParameteri: _emscripten_glProgramParameteri,
  emscripten_glQueryCounterEXT: _emscripten_glQueryCounterEXT,
  emscripten_glReadBuffer: _emscripten_glReadBuffer,
  emscripten_glReadPixels: _emscripten_glReadPixels,
  emscripten_glReleaseShaderCompiler: _emscripten_glReleaseShaderCompiler,
  emscripten_glRenderbufferStorage: _emscripten_glRenderbufferStorage,
  emscripten_glRenderbufferStorageMultisample: _emscripten_glRenderbufferStorageMultisample,
  emscripten_glResumeTransformFeedback: _emscripten_glResumeTransformFeedback,
  emscripten_glSampleCoverage: _emscripten_glSampleCoverage,
  emscripten_glSamplerParameterf: _emscripten_glSamplerParameterf,
  emscripten_glSamplerParameterfv: _emscripten_glSamplerParameterfv,
  emscripten_glSamplerParameteri: _emscripten_glSamplerParameteri,
  emscripten_glSamplerParameteriv: _emscripten_glSamplerParameteriv,
  emscripten_glScissor: _emscripten_glScissor,
  emscripten_glShaderBinary: _emscripten_glShaderBinary,
  emscripten_glShaderSource: _emscripten_glShaderSource,
  emscripten_glStencilFunc: _emscripten_glStencilFunc,
  emscripten_glStencilFuncSeparate: _emscripten_glStencilFuncSeparate,
  emscripten_glStencilMask: _emscripten_glStencilMask,
  emscripten_glStencilMaskSeparate: _emscripten_glStencilMaskSeparate,
  emscripten_glStencilOp: _emscripten_glStencilOp,
  emscripten_glStencilOpSeparate: _emscripten_glStencilOpSeparate,
  emscripten_glTexImage2D: _emscripten_glTexImage2D,
  emscripten_glTexImage3D: _emscripten_glTexImage3D,
  emscripten_glTexParameterf: _emscripten_glTexParameterf,
  emscripten_glTexParameterfv: _emscripten_glTexParameterfv,
  emscripten_glTexParameteri: _emscripten_glTexParameteri,
  emscripten_glTexParameteriv: _emscripten_glTexParameteriv,
  emscripten_glTexStorage2D: _emscripten_glTexStorage2D,
  emscripten_glTexStorage3D: _emscripten_glTexStorage3D,
  emscripten_glTexSubImage2D: _emscripten_glTexSubImage2D,
  emscripten_glTexSubImage3D: _emscripten_glTexSubImage3D,
  emscripten_glTransformFeedbackVaryings: _emscripten_glTransformFeedbackVaryings,
  emscripten_glUniform1f: _emscripten_glUniform1f,
  emscripten_glUniform1fv: _emscripten_glUniform1fv,
  emscripten_glUniform1i: _emscripten_glUniform1i,
  emscripten_glUniform1iv: _emscripten_glUniform1iv,
  emscripten_glUniform1ui: _emscripten_glUniform1ui,
  emscripten_glUniform1uiv: _emscripten_glUniform1uiv,
  emscripten_glUniform2f: _emscripten_glUniform2f,
  emscripten_glUniform2fv: _emscripten_glUniform2fv,
  emscripten_glUniform2i: _emscripten_glUniform2i,
  emscripten_glUniform2iv: _emscripten_glUniform2iv,
  emscripten_glUniform2ui: _emscripten_glUniform2ui,
  emscripten_glUniform2uiv: _emscripten_glUniform2uiv,
  emscripten_glUniform3f: _emscripten_glUniform3f,
  emscripten_glUniform3fv: _emscripten_glUniform3fv,
  emscripten_glUniform3i: _emscripten_glUniform3i,
  emscripten_glUniform3iv: _emscripten_glUniform3iv,
  emscripten_glUniform3ui: _emscripten_glUniform3ui,
  emscripten_glUniform3uiv: _emscripten_glUniform3uiv,
  emscripten_glUniform4f: _emscripten_glUniform4f,
  emscripten_glUniform4fv: _emscripten_glUniform4fv,
  emscripten_glUniform4i: _emscripten_glUniform4i,
  emscripten_glUniform4iv: _emscripten_glUniform4iv,
  emscripten_glUniform4ui: _emscripten_glUniform4ui,
  emscripten_glUniform4uiv: _emscripten_glUniform4uiv,
  emscripten_glUniformBlockBinding: _emscripten_glUniformBlockBinding,
  emscripten_glUniformMatrix2fv: _emscripten_glUniformMatrix2fv,
  emscripten_glUniformMatrix2x3fv: _emscripten_glUniformMatrix2x3fv,
  emscripten_glUniformMatrix2x4fv: _emscripten_glUniformMatrix2x4fv,
  emscripten_glUniformMatrix3fv: _emscripten_glUniformMatrix3fv,
  emscripten_glUniformMatrix3x2fv: _emscripten_glUniformMatrix3x2fv,
  emscripten_glUniformMatrix3x4fv: _emscripten_glUniformMatrix3x4fv,
  emscripten_glUniformMatrix4fv: _emscripten_glUniformMatrix4fv,
  emscripten_glUniformMatrix4x2fv: _emscripten_glUniformMatrix4x2fv,
  emscripten_glUniformMatrix4x3fv: _emscripten_glUniformMatrix4x3fv,
  emscripten_glUnmapBuffer: _emscripten_glUnmapBuffer,
  emscripten_glUseProgram: _emscripten_glUseProgram,
  emscripten_glValidateProgram: _emscripten_glValidateProgram,
  emscripten_glVertexAttrib1f: _emscripten_glVertexAttrib1f,
  emscripten_glVertexAttrib1fv: _emscripten_glVertexAttrib1fv,
  emscripten_glVertexAttrib2f: _emscripten_glVertexAttrib2f,
  emscripten_glVertexAttrib2fv: _emscripten_glVertexAttrib2fv,
  emscripten_glVertexAttrib3f: _emscripten_glVertexAttrib3f,
  emscripten_glVertexAttrib3fv: _emscripten_glVertexAttrib3fv,
  emscripten_glVertexAttrib4f: _emscripten_glVertexAttrib4f,
  emscripten_glVertexAttrib4fv: _emscripten_glVertexAttrib4fv,
  emscripten_glVertexAttribDivisor: _emscripten_glVertexAttribDivisor,
  emscripten_glVertexAttribDivisorANGLE: _emscripten_glVertexAttribDivisorANGLE,
  emscripten_glVertexAttribDivisorARB: _emscripten_glVertexAttribDivisorARB,
  emscripten_glVertexAttribDivisorEXT: _emscripten_glVertexAttribDivisorEXT,
  emscripten_glVertexAttribDivisorNV: _emscripten_glVertexAttribDivisorNV,
  emscripten_glVertexAttribI4i: _emscripten_glVertexAttribI4i,
  emscripten_glVertexAttribI4iv: _emscripten_glVertexAttribI4iv,
  emscripten_glVertexAttribI4ui: _emscripten_glVertexAttribI4ui,
  emscripten_glVertexAttribI4uiv: _emscripten_glVertexAttribI4uiv,
  emscripten_glVertexAttribIPointer: _emscripten_glVertexAttribIPointer,
  emscripten_glVertexAttribPointer: _emscripten_glVertexAttribPointer,
  emscripten_glViewport: _emscripten_glViewport,
  emscripten_glWaitSync: _emscripten_glWaitSync,
  emscripten_has_asyncify: _emscripten_has_asyncify,
  emscripten_memcpy_big: _emscripten_memcpy_big,
  emscripten_request_fullscreen_strategy: _emscripten_request_fullscreen_strategy,
  emscripten_request_pointerlock: _emscripten_request_pointerlock,
  emscripten_resize_heap: _emscripten_resize_heap,
  emscripten_sample_gamepad_data: _emscripten_sample_gamepad_data,
  emscripten_set_beforeunload_callback_on_thread: _emscripten_set_beforeunload_callback_on_thread,
  emscripten_set_blur_callback_on_thread: _emscripten_set_blur_callback_on_thread,
  emscripten_set_canvas_element_size: _emscripten_set_canvas_element_size,
  emscripten_set_element_css_size: _emscripten_set_element_css_size,
  emscripten_set_focus_callback_on_thread: _emscripten_set_focus_callback_on_thread,
  emscripten_set_fullscreenchange_callback_on_thread: _emscripten_set_fullscreenchange_callback_on_thread,
  emscripten_set_gamepadconnected_callback_on_thread: _emscripten_set_gamepadconnected_callback_on_thread,
  emscripten_set_gamepaddisconnected_callback_on_thread: _emscripten_set_gamepaddisconnected_callback_on_thread,
  emscripten_set_keydown_callback_on_thread: _emscripten_set_keydown_callback_on_thread,
  emscripten_set_keypress_callback_on_thread: _emscripten_set_keypress_callback_on_thread,
  emscripten_set_keyup_callback_on_thread: _emscripten_set_keyup_callback_on_thread,
  emscripten_set_main_loop_arg: _emscripten_set_main_loop_arg,
  emscripten_set_mousedown_callback_on_thread: _emscripten_set_mousedown_callback_on_thread,
  emscripten_set_mouseenter_callback_on_thread: _emscripten_set_mouseenter_callback_on_thread,
  emscripten_set_mouseleave_callback_on_thread: _emscripten_set_mouseleave_callback_on_thread,
  emscripten_set_mousemove_callback_on_thread: _emscripten_set_mousemove_callback_on_thread,
  emscripten_set_mouseup_callback_on_thread: _emscripten_set_mouseup_callback_on_thread,
  emscripten_set_pointerlockchange_callback_on_thread: _emscripten_set_pointerlockchange_callback_on_thread,
  emscripten_set_resize_callback_on_thread: _emscripten_set_resize_callback_on_thread,
  emscripten_set_touchcancel_callback_on_thread: _emscripten_set_touchcancel_callback_on_thread,
  emscripten_set_touchend_callback_on_thread: _emscripten_set_touchend_callback_on_thread,
  emscripten_set_touchmove_callback_on_thread: _emscripten_set_touchmove_callback_on_thread,
  emscripten_set_touchstart_callback_on_thread: _emscripten_set_touchstart_callback_on_thread,
  emscripten_set_visibilitychange_callback_on_thread: _emscripten_set_visibilitychange_callback_on_thread,
  emscripten_set_wheel_callback_on_thread: _emscripten_set_wheel_callback_on_thread,
  emscripten_set_window_title: _emscripten_set_window_title,
  emscripten_sleep: _emscripten_sleep,
  environ_get: _environ_get,
  environ_sizes_get: _environ_sizes_get,
  exit: _exit,
  fd_close: _fd_close,
  fd_read: _fd_read,
  fd_seek: _fd_seek,
  fd_write: _fd_write,
  glActiveTexture: _glActiveTexture,
  glAttachShader: _glAttachShader,
  glBindBuffer: _glBindBuffer,
  glBindTexture: _glBindTexture,
  glBindVertexArray: _glBindVertexArray,
  glBlendFunc: _glBlendFunc,
  glBufferData: _glBufferData,
  glClear: _glClear,
  glClearColor: _glClearColor,
  glCompileShader: _glCompileShader,
  glCreateProgram: _glCreateProgram,
  glCreateShader: _glCreateShader,
  glCullFace: _glCullFace,
  glDeleteShader: _glDeleteShader,
  glDeleteTextures: _glDeleteTextures,
  glDisable: _glDisable,
  glDrawArrays: _glDrawArrays,
  glDrawElements: _glDrawElements,
  glEnable: _glEnable,
  glEnableVertexAttribArray: _glEnableVertexAttribArray,
  glFlush: _glFlush,
  glGenBuffers: _glGenBuffers,
  glGenTextures: _glGenTextures,
  glGenVertexArrays: _glGenVertexArrays,
  glGenerateMipmap: _glGenerateMipmap,
  glGetIntegerv: _glGetIntegerv,
  glGetProgramInfoLog: _glGetProgramInfoLog,
  glGetProgramiv: _glGetProgramiv,
  glGetShaderInfoLog: _glGetShaderInfoLog,
  glGetShaderiv: _glGetShaderiv,
  glGetUniformLocation: _glGetUniformLocation,
  glLinkProgram: _glLinkProgram,
  glPixelStorei: _glPixelStorei,
  glShaderSource: _glShaderSource,
  glTexImage2D: _glTexImage2D,
  glTexParameteri: _glTexParameteri,
  glUniform1f: _glUniform1f,
  glUniform1i: _glUniform1i,
  glUniform3fv: _glUniform3fv,
  glUniform4fv: _glUniform4fv,
  glUniformMatrix4fv: _glUniformMatrix4fv,
  glUseProgram: _glUseProgram,
  glVertexAttribPointer: _glVertexAttribPointer,
  glViewport: _glViewport,
  invoke_i: invoke_i,
  invoke_ii: invoke_ii,
  invoke_iii: invoke_iii,
  invoke_iiii: invoke_iiii,
  invoke_iiiii: invoke_iiiii,
  invoke_iiiiii: invoke_iiiiii,
  invoke_iiiiiiiii: invoke_iiiiiiiii,
  invoke_iiiiiiiiii: invoke_iiiiiiiiii,
  invoke_ji: invoke_ji,
  invoke_jiji: invoke_jiji,
  invoke_vi: invoke_vi,
  invoke_vii: invoke_vii,
  invoke_viii: invoke_viii,
  invoke_viiii: invoke_viiii,
  strftime_l: _strftime_l
};
var asm = createWasm();
var ___wasm_call_ctors = createExportWrapper('__wasm_call_ctors');
var _main = Module['_main'] = createExportWrapper('__main_argc_argv');
var _free = createExportWrapper('free');
var _malloc = createExportWrapper('malloc');
var setTempRet0 = createExportWrapper('setTempRet0');
var ___errno_location = createExportWrapper('__errno_location');
var _fflush = Module['_fflush'] = createExportWrapper('fflush');
var _fileno = createExportWrapper('fileno');
var _emscripten_builtin_memalign = createExportWrapper('emscripten_builtin_memalign');
var _setThrew = createExportWrapper('setThrew');
var _emscripten_stack_init = () => (_emscripten_stack_init = wasmExports['emscripten_stack_init'])();
var _emscripten_stack_get_free = () => (_emscripten_stack_get_free = wasmExports['emscripten_stack_get_free'])();
var _emscripten_stack_get_base = () => (_emscripten_stack_get_base = wasmExports['emscripten_stack_get_base'])();
var _emscripten_stack_get_end = () => (_emscripten_stack_get_end = wasmExports['emscripten_stack_get_end'])();
var stackSave = createExportWrapper('stackSave');
var stackRestore = createExportWrapper('stackRestore');
var stackAlloc = createExportWrapper('stackAlloc');
var _emscripten_stack_get_current = () => (_emscripten_stack_get_current = wasmExports['emscripten_stack_get_current'])();
var ___cxa_is_pointer_type = createExportWrapper('__cxa_is_pointer_type');
var dynCall_ji = Module['dynCall_ji'] = createExportWrapper('dynCall_ji');
var dynCall_jiji = Module['dynCall_jiji'] = createExportWrapper('dynCall_jiji');
var dynCall_iiji = Module['dynCall_iiji'] = createExportWrapper('dynCall_iiji');
var dynCall_viijii = Module['dynCall_viijii'] = createExportWrapper('dynCall_viijii');
var dynCall_iiiiij = Module['dynCall_iiiiij'] = createExportWrapper('dynCall_iiiiij');
var dynCall_iiiiijj = Module['dynCall_iiiiijj'] = createExportWrapper('dynCall_iiiiijj');
var dynCall_iiiiiijj = Module['dynCall_iiiiiijj'] = createExportWrapper('dynCall_iiiiiijj');

function invoke_ii(index,a1) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1);
  } catch(e) {
    stackRestore(sp);
    if (e !== e+0) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iiiii(index,a1,a2,a3,a4) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3,a4);
  } catch(e) {
    stackRestore(sp);
    if (e !== e+0) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iiii(index,a1,a2,a3) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3);
  } catch(e) {
    stackRestore(sp);
    if (e !== e+0) throw e;
    _setThrew(1, 0);
  }
}

function invoke_vi(index,a1) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1);
  } catch(e) {
    stackRestore(sp);
    if (e !== e+0) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iii(index,a1,a2) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2);
  } catch(e) {
    stackRestore(sp);
    if (e !== e+0) throw e;
    _setThrew(1, 0);
  }
}

function invoke_viii(index,a1,a2,a3) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3);
  } catch(e) {
    stackRestore(sp);
    if (e !== e+0) throw e;
    _setThrew(1, 0);
  }
}

function invoke_vii(index,a1,a2) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2);
  } catch(e) {
    stackRestore(sp);
    if (e !== e+0) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iiiiiiiiii(index,a1,a2,a3,a4,a5,a6,a7,a8,a9) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6,a7,a8,a9);
  } catch(e) {
    stackRestore(sp);
    if (e !== e+0) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iiiiii(index,a1,a2,a3,a4,a5) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3,a4,a5);
  } catch(e) {
    stackRestore(sp);
    if (e !== e+0) throw e;
    _setThrew(1, 0);
  }
}

function invoke_iiiiiiiii(index,a1,a2,a3,a4,a5,a6,a7,a8) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)(a1,a2,a3,a4,a5,a6,a7,a8);
  } catch(e) {
    stackRestore(sp);
    if (e !== e+0) throw e;
    _setThrew(1, 0);
  }
}

function invoke_i(index) {
  var sp = stackSave();
  try {
    return getWasmTableEntry(index)();
  } catch(e) {
    stackRestore(sp);
    if (e !== e+0) throw e;
    _setThrew(1, 0);
  }
}

function invoke_viiii(index,a1,a2,a3,a4) {
  var sp = stackSave();
  try {
    getWasmTableEntry(index)(a1,a2,a3,a4);
  } catch(e) {
    stackRestore(sp);
    if (e !== e+0) throw e;
    _setThrew(1, 0);
  }
}

function invoke_ji(index,a1) {
  var sp = stackSave();
  try {
    return dynCall_ji(index,a1);
  } catch(e) {
    stackRestore(sp);
    if (e !== e+0) throw e;
    _setThrew(1, 0);
  }
}

function invoke_jiji(index,a1,a2,a3,a4) {
  var sp = stackSave();
  try {
    return dynCall_jiji(index,a1,a2,a3,a4);
  } catch(e) {
    stackRestore(sp);
    if (e !== e+0) throw e;
    _setThrew(1, 0);
  }
}


// include: postamble.js
// === Auto-generated postamble setup entry stuff ===

// include: base64Utils.js
// Converts a string of base64 into a byte array.
// Throws error on invalid input.
function intArrayFromBase64(s) {
  if (typeof ENVIRONMENT_IS_NODE != 'undefined' && ENVIRONMENT_IS_NODE) {
    var buf = Buffer.from(s, 'base64');
    return new Uint8Array(buf['buffer'], buf['byteOffset'], buf['byteLength']);
  }

  try {
    var decoded = atob(s);
    var bytes = new Uint8Array(decoded.length);
    for (var i = 0 ; i < decoded.length ; ++i) {
      bytes[i] = decoded.charCodeAt(i);
    }
    return bytes;
  } catch (_) {
    throw new Error('Converting base64 string to bytes failed.');
  }
}

// If filename is a base64 data URI, parses and returns data (Buffer on node,
// Uint8Array otherwise). If filename is not a base64 data URI, returns undefined.
function tryParseAsDataURI(filename) {
  if (!isDataURI(filename)) {
    return;
  }

  return intArrayFromBase64(filename.slice(dataURIPrefix.length));
}
// end include: base64Utils.js
Module['addRunDependency'] = addRunDependency;
Module['removeRunDependency'] = removeRunDependency;
Module['FS_createPath'] = FS.createPath;
Module['FS_createDataFile'] = FS.createDataFile;
Module['FS_createLazyFile'] = FS.createLazyFile;
Module['FS_createDevice'] = FS.createDevice;
Module['FS_unlink'] = FS.unlink;
Module['FS_createPreloadedFile'] = FS.createPreloadedFile;
var missingLibrarySymbols = [
  'writeI53ToI64Clamped',
  'writeI53ToI64Signaling',
  'writeI53ToU64Clamped',
  'writeI53ToU64Signaling',
  'convertU32PairToI53',
  'ydayFromDate',
  'inetPton4',
  'inetNtop4',
  'inetPton6',
  'inetNtop6',
  'readSockaddr',
  'writeSockaddr',
  'getHostByName',
  'getCallstack',
  'emscriptenLog',
  'convertPCtoSourceLocation',
  'jstoi_s',
  'getDynCaller',
  'runtimeKeepalivePush',
  'runtimeKeepalivePop',
  'asmjsMangle',
  'handleAllocatorInit',
  'HandleAllocator',
  'getNativeTypeSize',
  'STACK_SIZE',
  'STACK_ALIGN',
  'POINTER_SIZE',
  'ASSERTIONS',
  'getCFunc',
  'ccall',
  'cwrap',
  'uleb128Encode',
  'sigToWasmTypes',
  'generateFuncType',
  'convertJsFunctionToWasm',
  'getEmptyTableSlot',
  'updateTableMap',
  'getFunctionAddress',
  'addFunction',
  'removeFunction',
  'reallyNegative',
  'unSign',
  'strLen',
  'reSign',
  'formatString',
  'intArrayToString',
  'AsciiToString',
  'UTF16ToString',
  'stringToUTF16',
  'lengthBytesUTF16',
  'UTF32ToString',
  'stringToUTF32',
  'lengthBytesUTF32',
  'fillDeviceOrientationEventData',
  'registerDeviceOrientationEventCallback',
  'fillDeviceMotionEventData',
  'registerDeviceMotionEventCallback',
  'screenOrientation',
  'fillOrientationChangeEventData',
  'registerOrientationChangeEventCallback',
  'hideEverythingExceptGivenElement',
  'restoreHiddenElements',
  'softFullscreenResizeWebGLRenderTarget',
  'registerPointerlockErrorEventCallback',
  'fillBatteryEventData',
  'battery',
  'registerBatteryEventCallback',
  'jsStackTrace',
  'stackTrace',
  'checkWasiClock',
  'wasiRightsToMuslOFlags',
  'wasiOFlagsToMuslOFlags',
  'createDyncallWrapper',
  'setImmediateWrapped',
  'clearImmediateWrapped',
  'polyfillSetImmediate',
  'getPromise',
  'makePromise',
  'idsToPromises',
  'makePromiseCallback',
  'findMatchingCatch',
  'getSocketFromFD',
  'getSocketAddress',
  '_setNetworkCallback',
  'writeGLArray',
  'registerWebGlEventCallback',
  'runAndAbortIfError',
  'GLFW_Window',
  'ALLOC_NORMAL',
  'ALLOC_STACK',
  'allocate',
  'writeStringToMemory',
  'writeAsciiToMemory',
];
missingLibrarySymbols.forEach(missingLibrarySymbol)

var unexportedSymbols = [
  'run',
  'addOnPreRun',
  'addOnInit',
  'addOnPreMain',
  'addOnExit',
  'addOnPostRun',
  'FS_createFolder',
  'FS_createLink',
  'out',
  'err',
  'callMain',
  'abort',
  'keepRuntimeAlive',
  'wasmMemory',
  'wasmTable',
  'wasmExports',
  'stackAlloc',
  'stackSave',
  'stackRestore',
  'getTempRet0',
  'setTempRet0',
  'writeStackCookie',
  'checkStackCookie',
  'writeI53ToI64',
  'readI53FromI64',
  'readI53FromU64',
  'convertI32PairToI53',
  'convertI32PairToI53Checked',
  'ptrToString',
  'zeroMemory',
  'exitJS',
  'getHeapMax',
  'growMemory',
  'ENV',
  'MONTH_DAYS_REGULAR',
  'MONTH_DAYS_LEAP',
  'MONTH_DAYS_REGULAR_CUMULATIVE',
  'MONTH_DAYS_LEAP_CUMULATIVE',
  'isLeapYear',
  'arraySum',
  'addDays',
  'ERRNO_CODES',
  'ERRNO_MESSAGES',
  'setErrNo',
  'DNS',
  'Protocols',
  'Sockets',
  'initRandomFill',
  'randomFill',
  'timers',
  'warnOnce',
  'UNWIND_CACHE',
  'readEmAsmArgsArray',
  'readEmAsmArgs',
  'runEmAsmFunction',
  'runMainThreadEmAsm',
  'jstoi_q',
  'getExecutableName',
  'listenOnce',
  'autoResumeAudioContext',
  'dynCallLegacy',
  'dynCall',
  'handleException',
  'callUserCallback',
  'maybeExit',
  'safeSetTimeout',
  'asyncLoad',
  'alignMemory',
  'mmapAlloc',
  'freeTableIndexes',
  'functionsInTableMap',
  'setValue',
  'getValue',
  'PATH',
  'PATH_FS',
  'UTF8Decoder',
  'UTF8ArrayToString',
  'UTF8ToString',
  'stringToUTF8Array',
  'stringToUTF8',
  'lengthBytesUTF8',
  'intArrayFromString',
  'stringToAscii',
  'UTF16Decoder',
  'stringToNewUTF8',
  'stringToUTF8OnStack',
  'writeArrayToMemory',
  'JSEvents',
  'registerKeyEventCallback',
  'specialHTMLTargets',
  'maybeCStringToJsString',
  'findEventTarget',
  'findCanvasEventTarget',
  'getBoundingClientRect',
  'fillMouseEventData',
  'registerMouseEventCallback',
  'registerWheelEventCallback',
  'registerUiEventCallback',
  'registerFocusEventCallback',
  'fillFullscreenChangeEventData',
  'registerFullscreenChangeEventCallback',
  'JSEvents_requestFullscreen',
  'JSEvents_resizeCanvasForFullscreen',
  'registerRestoreOldStyle',
  'setLetterbox',
  'currentFullscreenStrategy',
  'restoreOldWindowedStyle',
  'doRequestFullscreen',
  'fillPointerlockChangeEventData',
  'registerPointerlockChangeEventCallback',
  'requestPointerLock',
  'fillVisibilityChangeEventData',
  'registerVisibilityChangeEventCallback',
  'registerTouchEventCallback',
  'fillGamepadEventData',
  'registerGamepadEventCallback',
  'registerBeforeUnloadEventCallback',
  'setCanvasElementSize',
  'getCanvasElementSize',
  'demangle',
  'demangleAll',
  'ExitStatus',
  'getEnvStrings',
  'doReadv',
  'doWritev',
  'promiseMap',
  'uncaughtExceptionCount',
  'exceptionLast',
  'exceptionCaught',
  'ExceptionInfo',
  'Browser',
  'setMainLoop',
  'wget',
  'SYSCALLS',
  'preloadPlugins',
  'FS_modeStringToFlags',
  'FS_getMode',
  'FS_stdin_getChar_buffer',
  'FS_stdin_getChar',
  'FS',
  'MEMFS',
  'TTY',
  'PIPEFS',
  'SOCKFS',
  'tempFixedLengthArray',
  'miniTempWebGLFloatBuffers',
  'miniTempWebGLIntBuffers',
  'heapObjectForWebGLType',
  'heapAccessShiftForWebGLHeap',
  'webgl_enable_ANGLE_instanced_arrays',
  'webgl_enable_OES_vertex_array_object',
  'webgl_enable_WEBGL_draw_buffers',
  'webgl_enable_WEBGL_multi_draw',
  'GL',
  'emscriptenWebGLGet',
  'computeUnpackAlignedImageSize',
  'colorChannelsInGlTextureFormat',
  'emscriptenWebGLGetTexPixelData',
  '__glGenObject',
  'emscriptenWebGLGetUniform',
  'webglGetUniformLocation',
  'webglPrepareUniformLocationsBeforeFirstUse',
  'webglGetLeftBracePos',
  'emscriptenWebGLGetVertexAttrib',
  '__glGetActiveAttribOrUniform',
  'emscriptenWebGLGetBufferBinding',
  'emscriptenWebGLValidateMapBufferTarget',
  'emscripten_webgl_power_preferences',
  'AL',
  'GLUT',
  'EGL',
  'GLEW',
  'IDBStore',
  'GLFW',
  'emscriptenWebGLGetIndexed',
  'webgl_enable_WEBGL_draw_instanced_base_vertex_base_instance',
  'webgl_enable_WEBGL_multi_draw_instanced_base_vertex_base_instance',
  'allocateUTF8',
  'allocateUTF8OnStack',
];
unexportedSymbols.forEach(unexportedRuntimeSymbol);



var calledRun;

dependenciesFulfilled = function runCaller() {
  // If run has never been called, and we should call run (INVOKE_RUN is true, and Module.noInitialRun is not false)
  if (!calledRun) run();
  if (!calledRun) dependenciesFulfilled = runCaller; // try this again later, after new deps are fulfilled
};

function callMain(args = []) {
  assert(runDependencies == 0, 'cannot call main when async dependencies remain! (listen on Module["onRuntimeInitialized"])');
  assert(__ATPRERUN__.length == 0, 'cannot call main when preRun functions remain to be called');

  var entryFunction = _main;

  args.unshift(thisProgram);

  var argc = args.length;
  var argv = stackAlloc((argc + 1) * 4);
  var argv_ptr = argv >> 2;
  args.forEach((arg) => {
    HEAP32[argv_ptr++] = stringToUTF8OnStack(arg);
  });
  HEAP32[argv_ptr] = 0;

  try {

    var ret = entryFunction(argc, argv);

    // if we're not running an evented main loop, it's time to exit
    exitJS(ret, /* implicit = */ true);
    return ret;
  }
  catch (e) {
    return handleException(e);
  }
}

function stackCheckInit() {
  // This is normally called automatically during __wasm_call_ctors but need to
  // get these values before even running any of the ctors so we call it redundantly
  // here.
  _emscripten_stack_init();
  // TODO(sbc): Move writeStackCookie to native to to avoid this.
  writeStackCookie();
}

function run(args = arguments_) {

  if (runDependencies > 0) {
    return;
  }

    stackCheckInit();

  preRun();

  // a preRun added a dependency, run will be called later
  if (runDependencies > 0) {
    return;
  }

  function doRun() {
    // run may have just been called through dependencies being fulfilled just in this very frame,
    // or while the async setStatus time below was happening
    if (calledRun) return;
    calledRun = true;
    Module['calledRun'] = true;

    if (ABORT) return;

    initRuntime();

    preMain();

    if (Module['onRuntimeInitialized']) Module['onRuntimeInitialized']();

    if (shouldRunNow) callMain(args);

    postRun();
  }

  if (Module['setStatus']) {
    Module['setStatus']('Running...');
    setTimeout(function() {
      setTimeout(function() {
        Module['setStatus']('');
      }, 1);
      doRun();
    }, 1);
  } else
  {
    doRun();
  }
  checkStackCookie();
}

function checkUnflushedContent() {
  // Compiler settings do not allow exiting the runtime, so flushing
  // the streams is not possible. but in ASSERTIONS mode we check
  // if there was something to flush, and if so tell the user they
  // should request that the runtime be exitable.
  // Normally we would not even include flush() at all, but in ASSERTIONS
  // builds we do so just for this check, and here we see if there is any
  // content to flush, that is, we check if there would have been
  // something a non-ASSERTIONS build would have not seen.
  // How we flush the streams depends on whether we are in SYSCALLS_REQUIRE_FILESYSTEM=0
  // mode (which has its own special function for this; otherwise, all
  // the code is inside libc)
  var oldOut = out;
  var oldErr = err;
  var has = false;
  out = err = (x) => {
    has = true;
  }
  try { // it doesn't matter if it fails
    _fflush(0);
    // also flush in the JS FS layer
    ['stdout', 'stderr'].forEach(function(name) {
      var info = FS.analyzePath('/dev/' + name);
      if (!info) return;
      var stream = info.object;
      var rdev = stream.rdev;
      var tty = TTY.ttys[rdev];
      if (tty && tty.output && tty.output.length) {
        has = true;
      }
    });
  } catch(e) {}
  out = oldOut;
  err = oldErr;
  if (has) {
    warnOnce('stdio streams had content in them that was not flushed. you should set EXIT_RUNTIME to 1 (see the Emscripten FAQ), or make sure to emit a newline when you printf etc.');
  }
}

if (Module['preInit']) {
  if (typeof Module['preInit'] == 'function') Module['preInit'] = [Module['preInit']];
  while (Module['preInit'].length > 0) {
    Module['preInit'].pop()();
  }
}

// shouldRunNow refers to calling main(), not run().
var shouldRunNow = true;

if (Module['noInitialRun']) shouldRunNow = false;

run();


// end include: postamble.js
