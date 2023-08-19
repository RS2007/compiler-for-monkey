let arr = [1,1,2];
let fib = fn(x){
  if(!!arr[x]) { return arr[x]; };
  let answer = fib(x-1)+fib(x-2);
  push(arr,answer);
  return answer;
}; 
puts("25th fibonnaci number",fib(25));
