cmd_/home/ns/OSConcepts/ProcessManagement/taskinfo/Module.symvers :=  sed 's/ko$$/o/'  /home/ns/OSConcepts/ProcessManagement/taskinfo/modules.order | scripts/mod/modpost -m     -o /home/ns/OSConcepts/ProcessManagement/taskinfo/Module.symvers -e -i Module.symvers -T - 