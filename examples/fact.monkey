let fact = fn(x){
  if(x == 1){
      return x;
  }
  return x*fact(x-1);
};
fact(5);
