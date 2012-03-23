#include <CoreServices/CoreServices.h>
#include "_cgo_export.h"

static void callback(ConstFSEventStreamRef streamRef, void *clientCallBackInfo, size_t numEvents, void *eventPaths, const FSEventStreamEventFlags eventFlags[], const FSEventStreamEventId eventIds[]);
static void callback(ConstFSEventStreamRef streamRef, void *clientCallBackInfo, size_t numEvents, void *eventPaths, const FSEventStreamEventFlags eventFlags[], const FSEventStreamEventId eventIds[]) {
  watchDirsCallback((FSEventStreamRef)streamRef, numEvents, eventPaths, (FSEventStreamEventFlags*)eventFlags);
}

FSEventStreamRef fswatch_stream_for_paths(char** paths, int paths_n) {
  CFMutableArrayRef pathsToWatch = CFArrayCreateMutable(NULL, 0, &kCFTypeArrayCallBacks);

  int i;
  for (i = 0; i < paths_n; i++)
    CFArrayAppendValue(pathsToWatch, CFStringCreateWithCString(NULL, paths[i], kCFStringEncodingUTF8));

  FSEventStreamRef stream = FSEventStreamCreate(NULL,
                                                callback,
                                                NULL,
                                                pathsToWatch,
                                                kFSEventStreamEventIdSinceNow,
                                                0.1,
                                                kFSEventStreamCreateFlagNoDefer | kFSEventStreamCreateFlagFileEvents);
  FSEventStreamScheduleWithRunLoop(stream, CFRunLoopGetCurrent(), kCFRunLoopCommonModes);

  return stream;
}

void fswatch_unwatch_stream(FSEventStreamRef stream) {
  FSEventStreamStop(stream);
  FSEventStreamInvalidate(stream);
  FSEventStreamRelease(stream);
}
