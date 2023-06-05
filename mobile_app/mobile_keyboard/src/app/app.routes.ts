import { Routes } from '@angular/router';

export const routes: Routes = [
  {
    path: '',
    redirectTo: 'folder/inbox',
    pathMatch: 'full',
  },
  {
    path: 'folder/:id',
    loadComponent: () =>
      import('./folder/folder.page').then((m) => m.FolderPage),
  },
  {
    path: 't1',
    loadComponent: () => import('./t1/t1.page').then( m => m.T1Page)
  },
  {
    path: 'new-pin',
    loadComponent: () => import('./new-pin/new-pin.page').then( m => m.NewPinPage)
  },
];
