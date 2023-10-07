cmd_/home/ns/OSConcepts/KernelModules101/hello.mod := printf '%s\n'   hello.o | awk '!x[$$0]++ { print("/home/ns/OSConcepts/KernelModules101/"$$0) }' > /home/ns/OSConcepts/KernelModules101/hello.mod
