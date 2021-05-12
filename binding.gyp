{
"libraries": [
    'src/AnomalyDetector.h'
    ],
  'targets': [
    {
      'target_name': 'test2-native',
      'sources': [ 'src/test2.cc', 'src/timeseries.cc', 'src/SimpleAnomalyDetector.cc', 'src/minCircle.cc', 'src/HybridAnomalyDetector.cc', 'src/anomaly_detection_util.cc'],
      'include_dirs': ["<!@(node -p \"require('node-addon-api').include\")"],
      'dependencies': ["<!(node -p \"require('node-addon-api').gyp\")"],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      'xcode_settings': {
        'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
        'CLANG_CXX_LIBRARY': 'libc++',
        'MACOSX_DEPLOYMENT_TARGET': '10.7'
      },
      'msvs_settings': {
        'VCCLCompilerTool': { 'ExceptionHandling': 1 },
      }
    }
  ]
}