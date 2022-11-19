# Manipulating-Generic-Vector

The implementation consists of storing the data in a "generic vector" void *arr. Each element is defined by a header that describes the content, followed by the actual data. For example, at the arr address the specific header structure is located, and at the arr + sizeof(struct header) address the actual data of the first element is located.

Passing the elements for adding to the vector is done through the data_structure structure. This contains the address to a  "struct head" variable in which the header of the added element is stored and an address to a generic data area. In the vector, both the header and the actual data will have to be copied, we don't want to store the pointers but the bytes from that address.

The implementations of head and data_structure is created in "structs.h"

The program receives input from keyboard until receiving the exit command, after which the program releases the memory and exit.

Orders received come in the following format:

insert dedicator type suma1 suma2 dedicated - the add_last function will be called
insert_at index dedicator type suma1 suma2 dedicated - the add_at function will be called
delete_at index - the delete_at function will be called
find index - the find function will be called
print - the entire vector will be displayed
exit - the memory will be freed and the program will be exited


# Developed functions 

1. add_last
int add_last(void **arr, int *len, data_structure *data);

The function adds an element to the end of the vector arr. It receives the address of the generic vector (arr), the address of the length of the vector represented in the number of elements in the vector (len), and the pointer to the structure to pass the element (data). The length is increased at the end of the operations. If the operations were completed successfully, then the function will return 0, otherwise it will return a value different from 0.

2. add_at
int add_at(void **arr, int *len, data_structure *data, int index)

Similar to the add_last function, add_at receives the address of the vector, the address of the length and the address of the data structure, but also the index at which the element is wanted to be stored. If the index is greater than 0, nothing will be added and an error will be returned. If it is greater than the length of the vector, it will be added to the end.

3.find
void find(void *data_block, int len, int index)

The function receives the data vector, its length and the index at which the display is desired. If the index is greater than the length or less than 0, nothing is displayed. Otherwise, the element from the respective index will be displayed according to the specified format

4.deleta_at
int delete_at(void **arr, int *len, int index)

The function receives the beginning of the vector, the number of current elements in the vector, and the index of the element which will be moved. The programm iterates through vector until that index and removes the element from the vector, taking care to move the remaining bits after the element, in place of the removed element.

5.print
print(void *arr, int len)

The print function receives a pointer to the data vector and the number of elements in the vector. It goes through the vector and prints the data inside the vector corresponding to the data type inside it.
