import requests
import shutil
import time
import os
import random
import sys
from PIL import Image


def get_home_path():
    return os.path.expanduser("~")


def get_file_path(wall_dir, getFromFav=False):
    file_name = ""
    if getFromFav:
        temp = os.listdir(wall_dir)
        r = random.randint(0, len(temp)-1)
        file_name = temp[r]
    else:
        file_name = str(int(time.time()))+".jpeg"

    return os.path.join(wall_dir, file_name)


def get_wallpaper_url():
    return "http://picsum.photos/3840/2400/?random"


def download_file(path):
    url = get_wallpaper_url()
    r = requests.get(url, stream=True)
    try:
        if r.status_code == 200:
            with open(path, 'wb') as f:
                r.raw.decode_content = True
                shutil.copyfileobj(r.raw, f)
        else:
            return False
    except:
        return False
    return True


def set_new_wallpaper(daily_wallaper_dir):
    download_file_path = get_file_path(daily_wallaper_dir)
    file_path = "file:"+download_file_path
    if download_file(download_file_path):
        os.system(
            "gsettings set org.gnome.desktop.background picture-uri "+file_path)
        w_path = os.path.join(get_home_path(), "dex_wall",
                              ".current_wallpaper")
        f = open(w_path, "w")
        f.write(download_file_path)
        f.close()
        print("Enjoy Your New Wallpaper !!!")
    else:
        print("Download Failed!!! Please Try Again Later...")


def set_fav_wallpaper(fav_wallaper_dir):
    file_path = "file:"+get_file_path(fav_wallaper_dir, getFromFav=True)
    if file_path:
        os.system(
            "gsettings set org.gnome.desktop.background picture-uri "+file_path)
        print("Enjoy Your Favourite Wallpaper !!!")
    else:
        print("Something Went Wrong!!! Please Try Again Later...")


def set_env(daily_wallaper_dir, fav_wallaper_dir):
    try:
        os.makedirs(daily_wallaper_dir)
        os.makedirs(fav_wallaper_dir)
    except Exception as e:
        pass


def copy_image(src, des):
    temp = open(src, "rb")
    wallpaper = Image.open(temp)
    wallpaper.save(des)


def save_fav(fav_wallaper_dir):
    w_path = os.path.join(get_home_path(), "dex_wall", ".current_wallpaper")
    f = open(w_path, "r")
    curr_path = f.readline().rstrip()
    f.close()
    path_fav = curr_path.replace("daily_wallpaper", "fav_wallpaper")
    if not os.path.exists(path_fav):
        if os.path.exists(curr_path):
            copy_image(curr_path, path_fav)
            print("Wallpaper Added to Favourite!!")
        else:
            print("Could Not Find Desired Wallpaper")
    else:
        print("Already Added to Favourite!")


if __name__=='__main__':
    home_path = get_home_path()
    daily_wallaper_dir = os.path.join(home_path, "dex_wall", "daily_wallpaper")
    fav_wallaper_dir = os.path.join(home_path, "dex_wall", "fav_wallpaper")
    set_env(daily_wallaper_dir, fav_wallaper_dir)
    if len(sys.argv) > 1:
        if sys.argv[1] == "addfav":
            save_fav(fav_wallaper_dir)
        if sys.argv[1] == "setfav":
            set_fav_wallpaper(fav_wallaper_dir)
    else:
        set_new_wallpaper(daily_wallaper_dir)