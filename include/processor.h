#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  

 private:
   float prevNonidle=0;       //prevIdle + prevNonIdle
   float prevIdle=0;
};

#endif
