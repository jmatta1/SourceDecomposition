NORTH_WALL = [[], ["Rx_Wall"],
              ["Rx_Wall_4_0","Rx_Wall_4_1","Rx_Wall_4_2","Rx_Wall_4_3"],
              ["Rx_Wall_9_0","Rx_Wall_9_1","Rx_Wall_9_2","Rx_Wall_9_3",
               "Rx_Wall_9_4","Rx_Wall_9_5","Rx_Wall_9_6","Rx_Wall_9_7",
               "Rx_Wall_9_8"],
              ["Rx_Wall_16_0","Rx_Wall_16_1","Rx_Wall_16_2","Rx_Wall_16_3",
               "Rx_Wall_16_4","Rx_Wall_16_5","Rx_Wall_16_6","Rx_Wall_16_7",
               "Rx_Wall_16_8","Rx_Wall_16_9","Rx_Wall_16_10","Rx_Wall_16_11",
               "Rx_Wall_16_12","Rx_Wall_16_13","Rx_Wall_16_14","Rx_Wall_16_15"]]

SOUTH_WALL = [[], ["MIF_Room_Wall"],
              ["MIF_Room_Wall_4_0","MIF_Room_Wall_4_1","MIF_Room_Wall_4_2","MIF_Room_Wall_4_3"],
              ["MIF_Room_Wall_9_0","MIF_Room_Wall_9_1","MIF_Room_Wall_9_2","MIF_Room_Wall_9_3",
               "MIF_Room_Wall_9_4","MIF_Room_Wall_9_5","MIF_Room_Wall_9_6","MIF_Room_Wall_9_7",
               "MIF_Room_Wall_9_8"],
              ["MIF_Room_Wall_16_0","MIF_Room_Wall_16_1","MIF_Room_Wall_16_2","MIF_Room_Wall_16_3",
               "MIF_Room_Wall_16_4","MIF_Room_Wall_16_5","MIF_Room_Wall_16_6","MIF_Room_Wall_16_7",
               "MIF_Room_Wall_16_8","MIF_Room_Wall_16_9","MIF_Room_Wall_16_10","MIF_Room_Wall_16_11",
               "MIF_Room_Wall_16_12","MIF_Room_Wall_16_13","MIF_Room_Wall_16_14","MIF_Room_Wall_16_15"]]

EAST_WALL = [[], ["East_Wall"],
             ["East_Wall_4_0","East_Wall_4_1","East_Wall_4_2","East_Wall_4_3"],
             ["East_Wall_9_0","East_Wall_9_1","East_Wall_9_2","East_Wall_9_3",
              "East_Wall_9_4","East_Wall_9_5","East_Wall_9_6","East_Wall_9_7",
              "East_Wall_9_8"],
             ["East_Wall_16_0","East_Wall_16_1","East_Wall_16_2","East_Wall_16_3",
              "East_Wall_16_4","East_Wall_16_5","East_Wall_16_6","East_Wall_16_7",
              "East_Wall_16_8","East_Wall_16_9","East_Wall_16_10","East_Wall_16_11",
              "East_Wall_16_12","East_Wall_16_13","East_Wall_16_14","East_Wall_16_15"]]

WEST_WALL = [[], ["West_Wall"],
             ["West_Wall_4_0","West_Wall_4_1","West_Wall_4_2","West_Wall_4_3"],
             ["West_Wall_9_0","West_Wall_9_1","West_Wall_9_2","West_Wall_9_3",
              "West_Wall_9_4","West_Wall_9_5","West_Wall_9_6","West_Wall_9_7",
              "West_Wall_9_8"],
             ["West_Wall_16_0","West_Wall_16_1","West_Wall_16_2","West_Wall_16_3",
              "West_Wall_16_4","West_Wall_16_5","West_Wall_16_6","West_Wall_16_7",
              "West_Wall_16_8","West_Wall_16_9","West_Wall_16_10","West_Wall_16_11",
              "West_Wall_16_12","West_Wall_16_13","West_Wall_16_14","West_Wall_16_15"]]

CEILING = [[], ["Ceiling"],
           ["Ceiling_4_0","Ceiling_4_1","Ceiling_4_2","Ceiling_4_3"],
           ["Ceiling_9_0","Ceiling_9_1","Ceiling_9_2","Ceiling_9_3",
            "Ceiling_9_4","Ceiling_9_5","Ceiling_9_6","Ceiling_9_7",
            "Ceiling_9_8"],
           ["Ceiling_16_0","Ceiling_16_1","Ceiling_16_2","Ceiling_16_3",
            "Ceiling_16_4","Ceiling_16_5","Ceiling_16_6","Ceiling_16_7",
            "Ceiling_16_8","Ceiling_16_9","Ceiling_16_10","Ceiling_16_11",
            "Ceiling_16_12","Ceiling_16_13","Ceiling_16_14","Ceiling_16_15"]]

FLOOR = [[], ["Floor"],
         ["Floor_4_0","Floor_4_1","Floor_4_2","Floor_4_3"],
         ["Floor_9_0","Floor_9_1","Floor_9_2","Floor_9_3",
          "Floor_9_4","Floor_9_5","Floor_9_6","Floor_9_7", "Floor_9_8"],
         ["Floor_16_0","Floor_16_1","Floor_16_2","Floor_16_3",
          "Floor_16_4","Floor_16_5","Floor_16_6","Floor_16_7",
          "Floor_16_8","Floor_16_9","Floor_16_10","Floor_16_11",
          "Floor_16_12","Floor_16_13","Floor_16_14","Floor_16_15"]]

WALLS = [NORTH_WALL, SOUTH_WALL, EAST_WALL, WEST_WALL, CEILING, FLOOR]

CORNERS = ["Front_Bottom_Right_Corner", "Front_Top_Left_Corner",
           "Front_Bottom_Right_Corner", "Front_Bottom_Left_Corner",
           "Back_Top_Right_Corner", "Back_Top_Left_Corner",
           "Back_Bottom_Right_Corner", "Back_Bottom_Left_Corner"]

EDGES = ["Front_Top_Edge", "Front_Bottom_Edge", "Front_Left_Edge", "Front_Right_Edge",
         "Back_Top_Edge", "Back_Bottom_Edge", "Back_Left_Edge", "Back_Right_Edge",
         "Top_Right_Edge", "Top_Left_Edge", "Bottom_Right_Edge", "Bottom_Left_Edge"]

REQUIRED = ["EF4_Cover", "HB3_Access", "HB4_Access", "HB3", "HB3a", "HB4"]
