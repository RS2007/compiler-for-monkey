let fib = fn(x){
  if(x < 2){ 
    x; 
  }
  return fib(x-1)+fib(x-2);
};
fib(23);
