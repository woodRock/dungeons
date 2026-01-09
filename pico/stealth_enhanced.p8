pico-8 version 36
__lua__
-- stealth mode - pico-8 enhanced
-- improved with patrols, vision cones, collisions

function _init()
	gs="play" -- play, won, caught
	t=0
	music(0)  -- start background music
	
	player={x=16,y=16,vx=0,vy=0,r=2,sneak=false,det=0,det_timer=0}
	exit={x=112,y=112,r=4}
	
	-- guards with patrol routes
	guards={
		{x=30,y=40,r=2,patrol={30,40,60,40,60,20,30,20},p=0,speed=0.3,alert=0,alert_timer=0,vision_dir=0},
		{x=80,y=60,r=2,patrol={80,60,100,60,100,90,80,90},p=0,speed=0.25,alert=0,alert_timer=0,vision_dir=0},
		{x=50,y=100,r=2,patrol={50,100,90,100,90,110,50,110},p=0,speed=0.3,alert=0,alert_timer=0,vision_dir=0}
	}
	
	-- hide spots
	spots={{x=20,y=80,r=8},{x=100,y=30,r=8},{x=60,y=75,r=8}}
	
	-- walls (rectangles)
	walls={{x=35,y=40,w=25,h=4},{x=75,y=15,w=4,h=35},{x=25,y=70,w=50,h=4},{x=95,y=75,w=30,h=4}}
end

function _update()
	t+=0.016
	
	if gs=="play" then
		local dx,dy=0,0
		if btn(0) then dx=-1 end
		if btn(1) then dx=1 end
		if btn(2) then dy=-1 end
		if btn(3) then dy=1 end
		if btnp(4) then player.sneak=not player.sneak end
		
		-- move player
		local spd=player.sneak and 0.5 or 1.2
		local nx,ny=player.x+dx*spd,player.y+dy*spd
		
		if not collides_wall(nx,ny,player.r) then
			player.x=nx
			player.y=ny
		end
		
		player.x=mid(8,player.x,120)
		player.y=mid(8,player.y,120)
		
		-- update guards
		for g in all(guards) do
			update_guard(g)
		end
		
		-- check detection
		check_detection()
		
		-- check win
		if dist(player.x,player.y,exit.x,exit.y)<exit.r+player.r then
			gs="won"
			sfx(2)  -- victory sound
		end
	elseif gs=="caught" or gs=="won" then
		if btnp(4) then _init() end
	end
end

function update_guard(g)
	-- move along patrol path
	local pair_count=#g.patrol/2
	local pair_idx=flr(g.p)%pair_count
	local next_pair_idx=(pair_idx+1)%pair_count
	
	local x1=g.patrol[pair_idx*2+1]
	local y1=g.patrol[pair_idx*2+2]
	local x2=g.patrol[next_pair_idx*2+1]
	local y2=g.patrol[next_pair_idx*2+2]
	
	local dx,dy=x2-x1,y2-y1
	local len=sqrt(dx*dx+dy*dy)
	if len>0 then
		g.x+=dx/len*g.speed
		g.y+=dy/len*g.speed
	end
	
	-- advance patrol
	g.p+=(g.speed/20)
	if g.p>=pair_count then g.p=0 end
	
	-- vision direction
	if len>0 then
		g.vision_dir=atan2(dy,dx)
	end
	
	-- decrease alert
	g.alert=max(0,g.alert-0.01)
	g.alert_timer=max(0,g.alert_timer-1)
end

function check_detection()
	player.det=0
	player.det_timer=max(0,player.det_timer-1)
	
	-- check if in hide spot
	local hidden=false
	for s in all(spots) do
		if dist(player.x,player.y,s.x,s.y)<s.r then hidden=true end
	end
	
	for g in all(guards) do
		local d=dist(player.x,player.y,g.x,g.y)
		local vis_range=player.sneak and 15 or 25
		if hidden then vis_range*=0.2 end
		
		-- check if in vision range
		if d<vis_range then
			-- check if in vision cone (60 degree angle)
			local angle_to_player=atan2(player.y-g.y,player.x-g.x)
			local angle_diff=angle_to_player-g.vision_dir
			
			-- normalize angle diff to -pi to pi
			while angle_diff>0.5 do angle_diff-=1 end
			while angle_diff<-0.5 do angle_diff+=1 end
			
			if abs(angle_diff)<0.3 then
				-- check line of sight
				if line_of_sight(g.x,g.y,player.x,player.y) then
					g.alert=1.0
					g.alert_timer=60
					if player.det==0 then
						sfx(0)  -- alert sound
					end
					player.det+=1
					player.det_timer=60
					
					if player.det_timer>120 then
						gs="caught"
						sfx(1)  -- caught sound
					end
				end
			end
		end
	end
end

function line_of_sight(x1,y1,x2,y2)
	-- check if line is blocked by walls
	local steps=10
	for i=0,steps do
		local t=i/steps
		local x=x1+(x2-x1)*t
		local y=y1+(y2-y1)*t
		if collides_wall(x,y,1) then return false end
	end
	return true
end

function collides_wall(x,y,r)
	for w in all(walls) do
		if x+r>w.x and x-r<w.x+w.w and y+r>w.y and y-r<w.y+w.h then
			return true
		end
	end
	return false
end

function dist(x1,y1,x2,y2)
	local dx,dy=x2-x1,y2-y1
	return sqrt(dx*dx+dy*dy)
end

function _draw()
	cls(1)
	
	-- draw walls
	for w in all(walls) do
		rectfill(w.x,w.y,w.x+w.w,w.y+w.h,5)
	end
	
	-- draw hide spots (dark shadows)
	for s in all(spots) do
		circfill(s.x,s.y,s.r,2)
	end
	
	-- draw exit
	circfill(exit.x,exit.y,exit.r,11)
	circfill(exit.x,exit.y,exit.r-1,10)
	
	-- draw guards with vision cones
	for g in all(guards) do
		-- draw vision cone when alert
		if g.alert>0.5 then
			local cone_range=25
			local cone_width=0.3
			
			-- draw cone sectors
			for angle=g.vision_dir-cone_width,g.vision_dir+cone_width,0.1 do
				local ex=g.x+cos(angle)*cone_range
				local ey=g.y+sin(angle)*cone_range
				line(g.x,g.y,ex,ey,8)
			end
		end
		
		-- draw guard
		local col=8
		if g.alert>0.7 then col=2 end
		circfill(g.x,g.y,g.r,col)
	end
	
	-- draw player
	local pc=3
	if player.det>0 then pc=2 end
	if player.sneak then pc=5 end
	circfill(player.x,player.y,player.r,pc)
	
	-- ui
	print("time:"..flr(t),5,5,7)
	if player.sneak then print("sneak",60,5,7) end
	if player.det>0 then 
		local det_pct=flr((player.det_timer/120)*100)
		print("detected "..det_pct.."%",65,5,2)
	end
	
	if gs=="won" then
		print("escaped!",50,55,11)
		print("time:"..flr(t).."s",50,65,7)
		print("z to restart",45,75,7)
	elseif gs=="caught" then
		print("caught by guard!",40,55,2)
		print("z to restart",45,65,7)
	else
		print("arrows:move z:sneak",5,122,7)
	end
end

__gfx__
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000

__sfx__
010100000b3601f3502f3503f3504f3505f360703707135081350913509135091350813506135041350213500135007300000000000000000000000000000000000000000000000000000000000000000000000000
01010000223002b30034300283006830083008330083308330873008730083308330873008730087308730087308830088308830088308830087308830088308330083308830088308730083309330088300000000000
010100001f5701f5702f57007270072700727007270072700d2700d27007270072700727007270072700727007270072700727007270072700727007270072700727007270072700727007270072700727007270
