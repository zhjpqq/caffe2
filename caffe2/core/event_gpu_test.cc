#include <gtest/gtest.h>
#include "caffe2/core/context.h"
#include "caffe2/core/context_gpu.h"
#include "caffe2/core/event.h"

namespace caffe2 {

TEST(EventCUDATest, EventBasics) {
  DeviceOption device_cpu;
  device_cpu.set_device_type(CPU);
  DeviceOption device_cuda;
  device_cuda.set_device_type(CUDA);

  CPUContext context_cpu(device_cpu);
  CUDAContext context_cuda(device_cuda);

  Event event_cpu(device_cpu);
  Event event_cuda(device_cuda);

  // CPU context and event interactions
  context_cpu.WaitEvent(event_cpu);
  context_cpu.Record(&event_cpu);
  event_cpu.Finish();
  event_cpu.Record(CPU, &context_cpu);
  event_cpu.Wait(CPU, &context_cpu);

  // CUDA context and event interactions
  context_cuda.SwitchToDevice();
  context_cuda.WaitEvent(event_cuda);
  context_cuda.Record(&event_cuda);
  event_cuda.Finish();
  event_cuda.Record(CUDA, &context_cuda);
  event_cuda.Wait(CUDA, &context_cuda);

  // CPU context waiting for CUDA event
  context_cpu.WaitEvent(event_cuda);

  // CUDA context waiting for CPU event
  context_cuda.WaitEvent(event_cpu);
}

} // namespace caffe2
