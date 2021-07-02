#ifndef __FeedbackProcessor_h__
#define __FeedbackProcessor_h__

#include <utility>
#include "SignalProcessor.h"
#include "DcBlockingFilter.h"

template<class Processor>
class FeedbackSignalProcessor : public Processor {
protected:
  FloatArray feedback_buffer;
  float feedback_amount;
public:
  template <typename... Args>
  FeedbackSignalProcessor(FloatArray buffer, Args&&... args) :
    Processor(std::forward<Args>(args)...), feedback_buffer(buffer), feedback_amount(0) {}
  void setFeedback(float amount){
    feedback_amount = amount;
  }
  float getFeedback(){
    return feedback_amount;
  }
  void process(FloatArray input, FloatArray output){
    feedback_buffer.multiply(feedback_amount);
    input.add(feedback_buffer);
    Processor::process(input, output);
    feedback_buffer.copyFrom(output);
  }
  template <typename... Args>
  static FeedbackSignalProcessor<Processor>* create(size_t blocksize, Args&&... args){
    return new FeedbackSignalProcessor<Processor>(FloatArray::create(blocksize), std::forward<Args>(args)...);
  }
  static void destroy(FeedbackSignalProcessor<Processor>* obj){
    FloatArray::destroy(obj->feedback_buffer);
    Processor::destroy(obj);
  }
};

class StereoFeedbackProcessor : public MultiSignalProcessor {
protected:
  SignalProcessor* processor_left;
  SignalProcessor* processor_right;
  FloatArray feedback_left;
  FloatArray feedback_right;
  float feedback_amount;
public:
  template <typename... Args>
  StereoFeedbackProcessor(SignalProcessor* left, SignalProcessor* right,
			  FloatArray fbl, FloatArray fbr) :
    processor_left(left), processor_right(right), feedback_left(fbl), feedback_right(fbr) {}
  void setFeedback(float amount){
    feedback_amount = amount;
  }
  float getFeedback(){
    return feedback_amount;
  }
  void process(AudioBuffer& input, AudioBuffer& output){
    FloatArray input_left = input.getSamples(LEFT_CHANNEL);
    FloatArray output_left = output.getSamples(LEFT_CHANNEL);
    feedback_left.multiply(feedback_amount);
    input_left.add(feedback_left);
    processor_left->process(input_left, output_left);
    FloatArray input_right = input.getSamples(RIGHT_CHANNEL);
    FloatArray output_right = output.getSamples(RIGHT_CHANNEL);
    feedback_right.multiply(feedback_amount);
    input_right.add(feedback_right);
    processor_right->process(input_right, output_right);
    feedback_left.copyFrom(output_left);
    feedback_right.copyFrom(output_right);
  }
  static StereoFeedbackProcessor* create(SignalProcessor* left, SignalProcessor* right, size_t blocksize){
    return new StereoFeedbackProcessor(left, right, FloatArray::create(blocksize), FloatArray::create(blocksize));
  }
  static void destroy(StereoFeedbackProcessor* obj){
    FloatArray::destroy(obj->feedback_left);
    FloatArray::destroy(obj->feedback_right);
    delete obj;
  }
};

class PingPongFeedbackProcessor : public MultiSignalProcessor {
protected:
  DcBlockingFilter dc_left;
  DcBlockingFilter dc_right;
  SignalProcessor* processor_left;
  SignalProcessor* processor_right;
  FloatArray feedback_left;
  FloatArray feedback_right;
  float feedback_amount;
public:
  template <typename... Args>
  PingPongFeedbackProcessor(SignalProcessor* left, SignalProcessor* right,
				  FloatArray fbl, FloatArray fbr) :
    processor_left(left), processor_right(right), feedback_left(fbl), feedback_right(fbr) {}
  void setFeedback(float amount){
    feedback_amount = amount;
  }
  float getFeedback(){
    return feedback_amount;
  }
  void process(AudioBuffer& input, AudioBuffer& output){
    FloatArray input_left = input.getSamples(LEFT_CHANNEL);
    FloatArray output_left = output.getSamples(LEFT_CHANNEL);
    feedback_left.multiply(feedback_amount);
    input_left.add(feedback_left);
    processor_left->process(input_left, output_left);
    FloatArray input_right = input.getSamples(RIGHT_CHANNEL);
    FloatArray output_right = output.getSamples(RIGHT_CHANNEL);
    feedback_right.multiply(feedback_amount);
    input_right.add(feedback_right);
    processor_right->process(input_right, output_right);
    feedback_left.copyFrom(output_right);
    dc_left.process(feedback_left);
    feedback_right.copyFrom(output_left);
    dc_right.process(feedback_right);
  }
  static PingPongFeedbackProcessor* create(SignalProcessor* left, SignalProcessor* right, size_t blocksize){
    return new PingPongFeedbackProcessor(left, right, FloatArray::create(blocksize), FloatArray::create(blocksize));
  }
  static void destroy(PingPongFeedbackProcessor* obj){
    FloatArray::destroy(obj->feedback_left);
    FloatArray::destroy(obj->feedback_right);
    delete obj;
  }
};

#endif // __FeedbackProcessor_h__
