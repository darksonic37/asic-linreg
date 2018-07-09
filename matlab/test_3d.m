training_set = [0, 0; 
                1, 2;
                2, 1;
                6, 7;
                3, 2; 
                4, 5; 
                3, 6; 
                2, 2;
                4, 4;
                7, 3;
                7, 5;
                9, 6;
                8, 9;
                11, 5;
                11, 12;
                12, 9;
                8, 5;
                10, 7;
                13, 15;
                20, 14;
                17, 17;
                16, 7;
                15, 3;
                20, 6;
                30, 17
                2, 10
                5, 10
                5, 16
                25, 12;
                23, 18];

y = [1; 
     3; 
     4;
     8;
     4;
     5; 
     6; 
     5;
     6;
     4;
     6;
     7;
     9;
     9;
     14;
     10;
     8;
     10;
     11;
     15;
     17;
     12;
     14;
     16;
     22;
     8;
     20;
     27;
     3;
     19];   

scatter3(training_set(:,1), training_set(:,2), y);
xlabel('X axis');
ylabel('Y label');
zlabel('Z label');