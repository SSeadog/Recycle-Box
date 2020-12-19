mat = [7, 0, 1, 6, 5]  # can, pls, gls, trsh, none

max_mat = -1
max_value = 0

if mat[0] > max_value:
    max_mat = 0
    max_value = mat[0]

if mat[1] > max_value:
    max_mat = 1
    max_value = mat[1]

if mat[2] > max_value:
    max_mat = 2
    max_value = mat[2]

if sum(mat[3:5]) > max_value:
    max_mat = 3
    max_value = sum(mat[3:5])

print(max_mat)
