# You can modify this file to implement your own algorithm

from constants import *

"""
You can use the following values from constants.py to check for the type of cell in the grid:
I = 1 -> Wall 
o = 2 -> Pellet (Small Dot)
e = 3 -> Empty
"""

def get_next_coordinate(grid, location):

    """
    Calculate the next coordinate for 6ix-pac to move to.
    Check if the next coordinate is a valid move.

    Parameters:
    - grid (list of lists): A 2D array representing the game board.
    - location (tuple): The current location of the 6ix-pac in the form (x, y).

    Returns:
    - list or tuple: 
        - If the next coordinate is valid, return the next coordinate in the form (x, y) or [x,y].
        - If the next coordinate is invalid, return None.
    """
    location_arg = location
    
    solution = bfs_helper(grid, location_arg) # a list of moves leading to the nearest pellet
    if solution != None:
        location = solution[-2]
    
    if grid[location[0]][location[1]] == I:
        # TODO: other conditions for an invalid move include moving diagonally or moving by more than one square
        print("you done fucked up")
        return None
    else:
        return location

def dfs_helper(grid, location, num_moves, prev_dir):
    """
    I gave up on this one for now.
    
    Parameters:
    - grid (list of lists): A 2D array representing the game board.
    - location (list): The current location of the traverser in the form (x, y).
    - prev_dir: the direction that the traverser just went in.
    - num_moves: the number of moves the traverser has made so far
    - original_dir: the first direction that the traverser moved in
    """
    # define the directions for our traverser to go in
    up_vec = [0, 1]
    down_vec = [0, -1]
    left_vec = [-1, 0]
    right_vec = [1, 0]
    directions = [up_vec, right_vec, down_vec, left_vec]
    valid_directions = []
    
    # recursive function that traverses the grid to find the closest pellet that can be reached legally
    
    # base case 
    if grid[location[0]][location[1]] == o:
        return num_moves    # We reached the nearest available pellet, so recurse back up and return the first move
                            # we traveled.
    else:
        # check which directions are valid for us to go in before making a move
        for direction in directions:
            if grid[location[0] + direction[0]][location[1] + direction[1]] == I: # Don't add this as a valid direction to search
                pass
            elif direction == prev_dir:
                pass
            else:
                valid_directions.append(direction) # Add this direction into the list of valid directions
        
        
        # find the best move
        best_score = 99999 # an int representing the number of moves to a given pellet
        
        for potential_dir in valid_directions:
            location = [location[0] + potential_dir[0], location[1] + potential_dir[1]]

            # save the first direction that we move in
            if num_moves == 0:
                first_dir = potential_dir
                
            score = dfs_helper(grid, location, num_moves + 1, potential_dir)
            
            if score < best_score:
                best_score = score
                best_dir = first_dir
            
            best_location = [location[0] + best_dir[0], location[1] + best_dir[1]]
            
        return best_location
    
def bfs_helper(grid, location):
    """
    Perform a breadth-first search to find the nearest pellet and return the next location pacman should
    move in to reach it
    
    Parameters:
    - grid (list of lists):  A 2D array representing the game board.
    - location: (list):  The starting location for the bfs search
    """
    # initialize variables later used to find the neighbours of the current square being searched
    down_vec = [0, 1]
    up_vec = [0, -1]
    left_vec = [-1, 0]
    right_vec = [1, 0]
    directions = [up_vec, right_vec, down_vec, left_vec]
    
    # initialize the queue of squares to be searched and dictionary that stores the parent relations to each square
    queue = []
    queue.append(location)
    parents = {}
    parents[location] = None
    while len(queue) != 0:
        current_square = queue.pop(0)
        if grid[current_square[0]][current_square[1]] == o:
            return getPath(parents, current_square)
        
        # find the valid neighbours to add into the queue and search
        neighbours = [] # should be equal to or less than 4
        for direction in directions:
            if grid[current_square[0] + direction[0]][current_square[1] + direction[1]] != I: 
                next_square = (current_square[0] + direction[0], current_square[1] + direction[1])
                neighbours.append(next_square)
        
        # if the current_square doesn't contain a pellet, search the pellet's valid neighbours
        neighbourIsParent = False
        for neighbour in neighbours:
            for key in parents:
                if key == neighbour:
                    neighbourIsParent = True
            if not neighbourIsParent:
                parents[neighbour] = current_square
                queue.append(neighbour)
            
def getPath(parents_dict, target):
    solution = []
    current_square = target
    while current_square != None:
        solution.append(current_square)
        current_square = parents_dict.get(current_square)
    return solution
                    
                    
                
 
