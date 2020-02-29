
/**
 *******************************************************************************
  *   @file NNManager.h
  *   @author Cozza Giorgio, Liu Qiaqi
  *   @date 08/04/19
  *   @version 1.0
  *   @brief Class in charge of managing a neural network instance.
 *******************************************************************************

  This is a free software released into public domain. Anyone is free to copy,
  modify, publish, use, compile or distribute this software, either in source
  code form or as compiled binary

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
  OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
  OTHER DEALINGS IN THE SOFTWARE.
********************************************************************************
 */


#ifndef _NN_CONFIG_H
#define _NN_CONFIG_H

#define NN_NAME "AOS_Activity_Recognition"

#define NUM_CLASSES 7
#define NUM_ACTIVITIES 6


static const char* movements[7] = { "RUNNING", "WALKING", "JUMPING", "STANDING", "SITTING", "SUPINE", "LYING ON SIDE"};
static const char* activities[6] = { "SOCCER", "BASKETBALL", "VOLLEYBALL", "GOLF", "SLEEPING", "STUDYING"};


#endif