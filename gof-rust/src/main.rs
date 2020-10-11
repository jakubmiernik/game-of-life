
use imgref::*;
use rand::Rng;
use std::{thread, time};

#[derive(Debug)]
struct Board {
    width: i32,
    height: i32,
    grid_cur: ImgVec<i32>,
}

impl Board {
    pub fn new(width: i32, height: i32) -> Board {
        Board {
            width: width,
            height: height,
            grid_cur: ImgVec::new(vec!(), width as usize, height as usize),
        }
    }

    pub fn random(&mut self) {
        let mut rng = rand::thread_rng();
        let n = self.grid_cur.height() * self.grid_cur.width();
        if self.grid_cur.buf().is_empty() {
            for _ in 0..n {
                self.grid_cur.buf_mut().push(rng.gen::<bool>() as i32);
            }
        }
        else {
            for elem in self.grid_cur.buf_mut() {
                *elem = rng.gen::<bool>() as i32;
            }
        }
    }

    fn clean_screen(&self) {
        print!("{esc}[2J{esc}[1;1H", esc = 27 as char);
    }

    pub fn print(&self) {
        for ii in 0..self.grid_cur.height() {
            for jj in 0..self.grid_cur.width() {
                if self.grid_cur[ii][jj] == 1 {
                    print!("O");
                }
                else {
                    print!(".");
                }
            }
            println!();
        }
        println!();
    }

    fn update(&mut self) {
        let grid_prev = self.grid_cur.clone();
        for yy in 1..(self.grid_cur.height()-1) as i32 {
            for xx in 1..(self.grid_cur.width()-1) as i32 {
                let mut neighbors = 0;
                for ii in -1..2 {
                    for jj in -1..2 {
                        if ii == 0 && jj == 0 {
                            continue;
                        }
                        if grid_prev[(yy+ii) as usize][(xx+jj) as usize] == 1 {
                            neighbors += 1;
                        }
                    }
                }
                if neighbors < 2 || neighbors > 3{
                    self.grid_cur[yy as usize][xx as usize] = 0;
                }
                else if neighbors == 3 {
                    self.grid_cur[yy as usize][xx as usize] = 1;
                }
            }
        }
    }

    pub fn play(&mut self) {
        loop {
            self.update(); 
            self.clean_screen();
            self.print();
            thread::sleep(time::Duration::from_secs(1));
        }
    }
}

fn main() {
    let mut board = Board::new(100,100);
    board.random();
    board.print();
    board.play();
}
