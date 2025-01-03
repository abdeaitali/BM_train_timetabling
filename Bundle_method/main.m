%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%  Main program: TTP using Lagrangian Relaxation
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%   Obs. Before running this code, please make sure
%%%   that the mex-files were generated
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%% Initialization
k_max = 5; % maximum number of iterations
k = 1; % current iteration number
B = 12; % number of spatial blocks
T = 24*60*2; % number of time step 
R = 5; % number of requests
P = zeros(1,R); % number of generated path per request
eps_tolerance = 1e-10; % tolerance level for ending the bundle phase
diff = -eps_tolerance;

%%% Parameters to store the iteration results
mu = zeros(B,T,k_max+1); %mu(:,:,1) = rand(B,T);% prices initially random
Phi = zeros(R,k_max); % dual objective function
g = zeros(B,T,R,k_max); % sub-gradient of the dual obj function
lambda = zeros(R,k_max); % multiplier for the convex combinations of paths
Path = zeros(R,k_max); % store index of the path which was chosen for a request at certain iteration

%%% Global param
global ids
global requests
global network
global ordering

%%% Read the network data (OBS. specify the absolute path with "/")
[ids, requests, network, ordering, P] = MexReadData('C:/Users/abde/Documents/GitHub/TimetablePO/data');

%%% Dual iteration
while (k <= k_max) && (diff <= -eps_tolerance)
    %%% Solve the shortes path (C++)
    [Phi(:,k), g(:,:,:,k), Path(:,k)] = MexShortestPathSeq(ids, requests, network, ordering, mu(:,:,k));
    
    %%% Compute the new prices (Matlab)
    [mu(:,:,k+1), lambda(:,1:k), diff] = bundle(mu(:,:,1:k), Phi(:,1:k), g(:,:,:,1:k)); 
    
    % next iteration
    k = k+1;
end

%%% Compute the fractional solutions
x = zeros(1,sum(P)); %% main binary variable of the IP problem
ind = 1; %% auxiliary index
for r=1:R % train requests
    for p=1:P(r) % paths
        sum = 0; % approximation of x(p_r)
        for k=1:k_max
           if Path(r,k) == p
              sum = sum + lambda(r,k);
           end
        end
        x(ind) = sum;
        ind =ind+1;
    end
end
disp(x);

%%% Rapid Branching



%%% Results
