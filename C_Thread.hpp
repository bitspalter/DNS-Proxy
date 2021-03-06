//////////////////////////////////////////////////////////////////////////////////
// [ Thread_Class_Header ] [ TEMPLATE ]
//////////////////////////////////////////////////////////////////////////////////
//
//
// [::Last modi: 12.11.16 L.ey (�~)::]
//
//
#ifndef _C_THREAD_H_
 #define _C_THREAD_H_

   #include <pthread.h>
  
//////////////////////////////////////////////////////////////////////////////////

   const int C_THREAD_READY = 0x01;
   const int C_THREAD_ERROR = 0x00;

//////////////////////////////////////////////////////////////////////////////////

template <class T> 
class C_Thread {
   public:

      C_Thread(){bRunning = false;};  
     ~C_Thread(){};

      ////////////////////////////////////////////////////////////////////
      // [ create ]
      ////////////////////////////////////////////////////////////////////
      int create(T* pClass, void (T::*pFunc)()){
	
         if(!pClass || !pFunc || bRunning == true) return(C_THREAD_ERROR);

         if(pthread_create(&thread_id, NULL, pThread, this) != 0) return(C_THREAD_ERROR);
	   
         bRunning = true;

         C_Thread::pClass = pClass; 
	     C_Thread::pFunc  = pFunc;

         return(C_THREAD_READY);
      }
      ////////////////////////////////////////////////////////////////////
      // [ terminate ]
      ////////////////////////////////////////////////////////////////////
      int terminate(){

         if(bRunning == false) return(C_THREAD_ERROR);

         if(pthread_cancel(thread_id) != 0) return(C_THREAD_ERROR);

         bRunning = false;
  
         return(C_THREAD_READY);
      }

      ////////////////////////////////////////////////////////////////////
      // [ GET ]
      ////////////////////////////////////////////////////////////////////
      
      bool get_bRunning(){return(bRunning);};
      void set_bRunning(bool bValue){bRunning = bValue;};
      
   private:

      ////////////////////////////////////////////////////////////////////
      // [ pThread ]
      ////////////////////////////////////////////////////////////////////
      static void* pThread(void* pSelf){
         ((C_Thread*)pSelf)->Thread();
         return(nullptr);
      }
      ////////////////////////////////////////////////////////////////////
      // [ _Thread ]
      ////////////////////////////////////////////////////////////////////
      void Thread(){
         (pClass->*pFunc)();
      }

      ////////////////////////////////////////////////////////////////////
      // [ PROPERTY ]
      ////////////////////////////////////////////////////////////////////
      T* pClass;

      pthread_t thread_id;

      bool bRunning;

      ////////////////////////////////////////////////////////////////////

      void (T::*pFunc)();
 };

#endif // _C_THREAD_H_
