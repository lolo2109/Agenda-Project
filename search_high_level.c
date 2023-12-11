int search_high_level(cell_list* l, int val){
    cell* temp=(*l).head[(*l).level];
    for(int i = (*l).level; i>=0; i--){
        cell* old_temp=(*l).head[i];
        int stop = cell_list_length(l, i);
        for (int j = (*temp).value/pow(2,i)-1; j < stop; j++) {
            if ((*temp).value == val) {
                //printf("the value %d is at the index %d of level %d", val, j, i);
                return 0;
            }
            if((*temp).value > val){
                if(temp == (*l).head[i]){
                    temp = (*l).head[i-1];
                    break;
                }
                temp = old_temp;
                break;
            }
            old_temp = temp;
            temp = (*temp).next[0];
        }
    }
    //printf("the value %d is not found in the list", val);
    return 0;
}
