mat = [7, 0, 1, 6, 5]  # can, pls, gls, trsh, none

max_value = 0
max_mat = -1

for i, m in enumerate(mat):
    if m > max_value:
        max_mat = i
        max_value = m

print(max_mat)
