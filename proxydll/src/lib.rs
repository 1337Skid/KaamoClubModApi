#![cfg(windows)]

extern crate core;

use winapi::shared::minwindef;
use winapi::shared::minwindef::{BOOL, DWORD, HINSTANCE, LPVOID, UINT, MAX_PATH};
use winapi::shared::d3d9;
use winapi::um::winnt::{LPCSTR};
use winapi::um::libloaderapi::{LoadLibraryA, GetProcAddress, GetModuleFileNameA};
use winapi::um::consoleapi;
use std::ptr;
use std::ffi::CString;
use std::mem;
use winapi::shared::d3d9::IDirect3D9;

type _D3DCreate9 =  extern "stdcall" fn(UINT) -> *mut d3d9::IDirect3D9;
type _D3DPERF_SetOptions = extern "stdcall" fn(DWORD);

static mut hOriginal: HINSTANCE = ptr::null_mut();
static mut pDirect3DCreate9: Option<_D3DCreate9> = None;
static mut pD3DPERF_SetOptions: Option<_D3DPERF_SetOptions> = None;

#[no_mangle]
pub unsafe extern "system" fn D3DPERF_SetOptions(dwOptions: DWORD) {
    match pD3DPERF_SetOptions {
        Some(func) => func(dwOptions),
        None => panic!("SetOptions panic")
    }
}

#[no_mangle]
pub unsafe extern "stdcall" fn Direct3DCreate9(SDKVersion: UINT) -> *mut IDirect3D9 {
    match pDirect3DCreate9 {
        Some(func) => func(SDKVersion),
        None => panic!("Direct3DCreate9 panic")
    }
}

#[no_mangle]
#[allow(non_snake_case, unused_variables)]
extern "system" fn DllMain(dll_module: HINSTANCE, call_reason: DWORD, reserved: LPVOID) -> BOOL {
    const DLL_PROCESS_ATTACH: DWORD = 1;
    const DLL_PROCESS_DETACH: DWORD = 0;

    match call_reason {
        DLL_PROCESS_ATTACH => initialize(),
        DLL_PROCESS_DETACH => (),
        _ => ()
    }

    minwindef::TRUE
}

fn initialize() {
    unsafe {
        hOriginal = LoadLibraryA(CString::new("C:\\Windows\\System32\\d3d9.dll").unwrap().as_ptr());
        
        if !hOriginal.is_null() {
            pDirect3DCreate9 = Some(mem::transmute(GetProcAddress(hOriginal, CString::new("Direct3DCreate9").unwrap().as_ptr())));
            pD3DPERF_SetOptions = Some(mem::transmute(GetProcAddress(hOriginal, CString::new("D3DPERF_SetOptions").unwrap().as_ptr())));
        } else {
            panic!("Couldn't find d3d9.dll on the system");
        }

        let mut buffer = [0i8; MAX_PATH];
        let len = GetModuleFileNameA(ptr::null_mut(), buffer.as_mut_ptr(), MAX_PATH as u32);
        
        if len > 0 {
            let slice = std::slice::from_raw_parts(buffer.as_ptr() as *const u8, len as usize);
            let path_str = String::from_utf8_lossy(slice).to_lowercase();

            if path_str.contains("gof2launcher.exe") {
                return;
            }
        }

        consoleapi::AllocConsole();
        println!("[*] Loading KaamoClubModAPI...");

        let payload_path = CString::new("kaamoclubmodapi.dll").unwrap();
        
        println!("[*] Loading core modapi dll...");
        
        let coremodapi = LoadLibraryA(payload_path.as_ptr());

        if coremodapi.is_null() {
            println!("[-] Failed to load the core modapi dll");
        } else {
            println!("[+] Successfully loaded core modapi dll at: {:?}", coremodapi);
        }
    }
}