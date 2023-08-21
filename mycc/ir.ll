; ModuleID = 'main'
source_filename = "main"

@num = global i32 0
@.str = constant [4 x i8] c"%d\0A\00"
@.str.1 = constant [4 x i8] c"%d\0A\00"

define internal void @main() {
entry:
  store i32 5, i32* @num, align 4
  %num = load i32, i32* @num, align 4
  %calltmp = call i32 @factorial(i32 %num)
  %printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i32 %calltmp)
  %num1 = load i32, i32* @num, align 4
  call void @printFibonacciSeries(i32 %num1)
  ret void
}

declare i32 @printf(i8*, ...)

define i32 @factorial(i32 %0) {
entry:
  %factorial = alloca i32, align 4
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %n1 = load i32, i32* %n, align 4
  %tmpsle = icmp sle i32 %n1, 1
  %ifcond = icmp ne i1 %tmpsle, false
  br i1 %ifcond, label %then, label %else

then:                                             ; preds = %entry
  store i32 1, i32* %factorial, align 4
  br label %ifcont

else:                                             ; preds = %entry
  %n2 = load i32, i32* %n, align 4
  %n3 = load i32, i32* %n, align 4
  %subtmpi = sub i32 %n3, 1
  %calltmp = call i32 @factorial(i32 %subtmpi)
  %multmpi = mul i32 %n2, %calltmp
  store i32 %multmpi, i32* %factorial, align 4
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  %factorial4 = load i32, i32* %factorial, align 4
  ret i32 %factorial4
}

define i32 @fibonacci(i32 %0) {
entry:
  %fibonacci = alloca i32, align 4
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %n1 = load i32, i32* %n, align 4
  %tmpsle = icmp sle i32 %n1, 2
  %ifcond = icmp ne i1 %tmpsle, false
  br i1 %ifcond, label %then, label %else

then:                                             ; preds = %entry
  store i32 1, i32* %fibonacci, align 4
  br label %ifcont

else:                                             ; preds = %entry
  %n2 = load i32, i32* %n, align 4
  %subtmpi = sub i32 %n2, 1
  %calltmp = call i32 @fibonacci(i32 %subtmpi)
  %n3 = load i32, i32* %n, align 4
  %subtmpi4 = sub i32 %n3, 2
  %calltmp5 = call i32 @fibonacci(i32 %subtmpi4)
  %addtmpi = add i32 %calltmp, %calltmp5
  store i32 %addtmpi, i32* %fibonacci, align 4
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  %fibonacci6 = load i32, i32* %fibonacci, align 4
  ret i32 %fibonacci6
}

define void @printFibonacciSeries(i32 %0) {
entry:
  %i = alloca i32, align 4
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %n1 = load i32, i32* %n, align 4
  store i32 1, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %loop, %entry
  %i2 = load i32, i32* %i, align 4
  %1 = icmp sle i32 %i2, %n1
  %forCond = icmp ne i1 %1, false
  br i1 %forCond, label %loop, label %afterLoop

loop:                                             ; preds = %cond
  %i3 = load i32, i32* %i, align 4
  %calltmp = call i32 @fibonacci(i32 %i3)
  %printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i32 0, i32 0), i32 %calltmp)
  %nextvar = add i32 %i2, 1
  store i32 %nextvar, i32* %i, align 4
  br label %cond

afterLoop:                                        ; preds = %cond
  ret void
}
