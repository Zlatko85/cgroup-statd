#include <iostream>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <sys/select.h>

#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <climits>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>

#include <dlt/dlt.h>


#define BUFFER_SIZE PIPE_BUF


DLT_DECLARE_CONTEXT(myContext);
using namespace std;


vector<string> cgroups;

int get_cgroups() {
    const char *PATH = "/mnt/cgroups/cpuacct/";
    DIR *dir = opendir(PATH);



    struct dirent *entry; //= readdir(dir);
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type != DT_DIR) continue;
        if (!strcmp(entry->d_name, ".")) continue;
        if (!strcmp(entry->d_name, "..")) continue;
        //printf("%s\n", entry->d_name);
        cgroups.push_back(entry->d_name);


    }
    closedir(dir);

    return 0;
}


int get_cpuacct_stat(const char *s) {
    DLT_REGISTER_CONTEXT(myContext, "stat", "Test Context for Logging");
    char PATH[256];
    strcpy(PATH, "/mnt/cgroups/cpuacct/");
    strcat(PATH,s);
    strcat(PATH,"/cpuacct.stat");
    FILE *fp;
    fp = fopen(PATH, "r");

    cout << "Process " << getpid() << " result " << fp << endl;
    cout << PATH<<endl;

    do {

        rewind(fp);

        int a, b;
        fscanf(fp, "user %d\nsystem %d", &a, &b);
        //DLT_LOG(myContext, DLT_LOG_WARN, DLT_INT(a), DLT_INT(b));
        cout <<"user="<<a<<"\tsystem="<<b<<endl;
        break;
        sleep(3);
        

    }
    while (1);


    fclose(fp);
    DLT_UNREGISTER_CONTEXT(myContext);
}


int main(int argc, char *argv[]) {
    DLT_REGISTER_APP("GOGO", "Test Application for Logging");



    printf("Cgoups:\n");
    get_cgroups();
    for (vector<string>::iterator it = cgroups.begin(); it != cgroups.end(); ++it) {
        cout  << *it << endl;
        get_cpuacct_stat(it->c_str());
    }



    


    DLT_UNREGISTER_APP();

    return (0);
}

