#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

emoji(f,a,s)char*s;
{
char c;
return f&1?a!=*s++?emoji(f,a,s):s[11]:f&2?a!=*s++?1+emoji(f,a,s):1:f&4?a--?
 putchar(*s),emoji(f,a,s):a:f&8?*s?emoji(8,32,(c=emoji(1,*s++,"Arjan Kenter. \no$../.\""),
 emoji(4,emoji(2,*s++,"POCnWAUvBVxRsoqatKJurgXYyDQbzhLwkNjdMTGeIScHFmpliZEf"),&c),s)):
 65:(emoji(8,34,"rgeQjPruaOnDaPeWrAaPnPrCnOrPaPnPjPrCaPrPnPrPaOrvaPndeOrAnOrPnOrP\
nOaPnPjPaOrPnPrPnPrPtPnPrAaPnBrnnsrnnBaPeOrCnPrOnCaPnOaPnPjPtPnAaPnPrPnPrCaPn\
BrAnxrAnVePrCnBjPrOnvrCnxrAnxrAnsrOnvjPrOnUrOnornnsrnnorOtCnCjPrCtPnCrnnirWtP\
nCjPrCaPnOtPrCnErAnOjPrOnvtPnnrCnNrnnRePjPrPtnrUnnrntPnbtPrAaPnCrnnOrPjPrRtPn\
CaPrWtCnKtPnOtPrBnCjPronCaPrVtPnOtOnAtnrxaPnCjPrqnnaPrtaOrsaPnCtPjPratPnnaPrA\
aPnAaPtPnnaPrvaPnnjPrKtPnWaOrWtOnnaPnWaPrCaPnntOjPrrtOnWanrOtPnCaPnBtCjPrYtOn\
UaOrPnVjPrwtnnxjPrMnBjPrTnUjP"),0);
}
int main()
{
  if (fork() == 0){  // implementation of fork() in C-program of choice

  return emoji(0,75,"mIWltouQJGsBniKYvTxODAfbUcFzSpMwNCHEgrdLaPkyVRjXeqZh");
}

else
{
    wait(NULL);  // implementation of wait() in C-program of choice
}
}
