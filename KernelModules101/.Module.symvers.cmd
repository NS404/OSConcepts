cmd_/home/ns/OSConcepts/KernelModules101/Module.symvers :=  sed 's/ko$$/o/'  /home/ns/OSConcepts/KernelModules101/modules.order | scripts/mod/modpost -m     -o /home/ns/OSConcepts/KernelModules101/Module.symvers -e -i Module.symvers -T - 